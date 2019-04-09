 #include "Serial.h"    
#include <process.h>    
#include <iostream>
#include <string>

const UINT SLEEP_TIME_INTERVAL = 5; // When serial no data, sleep time interval to the next queries. (unit: second)
IOBuf g_IOBuf{};
int   g_sendNext{};

bool CSerialPort::s_bExit = false;

void IOBuf::PushBack(const std::string& str)
{
    if (str.size() > BUFFER_SIZE / 2)
    {
        PushBack(">> ERROR: str.size() > BUFFER_SIZE / 2!\n");
        return;
    }

    if ((strlen(output) + str.size()) > (int)(BUFFER_SIZE * 0.75)) // While the number of characters is more than 75% clear data 当字符数量超过百分之75时清除数据
    {
        int deletNumbers = 0;
        while (!(deletNumbers > (int)(BUFFER_SIZE * 0.15) && output[deletNumbers - 1] == '\n')) // Choose to remove more than 15% of the data line by line 逐行选择要清除的大于百分之15的数据
        {
            if (output[deletNumbers] == '\0') { break; } // If the output data are all selected, break 如果输出数据被全部选中了，则跳出
            ++deletNumbers;
        }

        size_t index = 0;
        size_t outputLen = strlen(output);
        for (size_t i = deletNumbers; i < outputLen; ++i)
        {
            output[index++] = output[i];
        }
        for (size_t i = index; i < outputLen; ++i)
        {
            output[i] = '\0';
        }
        outputIndex = strlen(output);
    }

    for (auto ch : str) { output[outputIndex++] = ch; }
    outputChange = true;
}

void IOBuf::PushBack(const size_t num)
{
    PushBack(std::to_string(num));
}

void IOBuf::PushBack(const int num)
{
    PushBack(std::to_string(num));
}

void IOBuf::Send(const std::string& str) 
{ 
    PushBack(str + '\n');

    memset(serialBuffer, '\0', strlen(serialBuffer) * sizeof(char));
    size_t index = 0;
    for (auto ch : str)
    {
        if (ch == '\n')
        {
            continue;
        }
        serialBuffer[index++] = ch;
    }
    // serialBuffer[index++] = '\0';
    serialBufferChange = true;
}

std::string WcharToChar(const wchar_t* wp, size_t m_encode = CP_ACP)
{
    std::string str;
    int len = WideCharToMultiByte((UINT)m_encode, 0, wp, (int)wcslen(wp), NULL, 0, NULL, NULL);
    char *m_char = new char[len + 1];
    WideCharToMultiByte((UINT)m_encode, 0, wp, (int)wcslen(wp), m_char, len, NULL, NULL);
    m_char[len] = '\0';
    str = m_char;
    delete m_char;
    return str;
}

CSerialPort::CSerialPort(void) : m_hListenThread(INVALID_HANDLE_VALUE)
{
    m_hComm = INVALID_HANDLE_VALUE;
    m_hListenThread = INVALID_HANDLE_VALUE;
    InitializeCriticalSection(&m_csCommunicationSync);
}

CSerialPort::~CSerialPort(void)
{
    CloseListenTread();
    ClosePort();
    DeleteCriticalSection(&m_csCommunicationSync);
}

bool CSerialPort::InitPort(UINT portNo, UINT baud, char parity, UINT databits, UINT stopsbits)
{
    // Temporary variables(临时变量), will develop parameters into a string, in order to(以) construct the DCB structure.
    char szDCBparam[50];
    sprintf_s(szDCBparam, "baud=%d parity=%c data=%d stop=%d", baud, parity, databits, stopsbits);

    // Open the specified(指定的) serial port, this function has a critical section internal protection(内部临界区保护), please do not add protection.
    if (!openPort(portNo))
    {
        return false;
    }

    // Enter the critical section.
    EnterCriticalSection(&m_csCommunicationSync);
    BOOL bIsSuccess = TRUE;

    // Set the timeout(超时时间) of serial ports, are set to 0, said don't use timeouts.
    COMMTIMEOUTS  CommTimeouts;
    CommTimeouts.ReadIntervalTimeout = 0;
    CommTimeouts.ReadTotalTimeoutMultiplier = 0;
    CommTimeouts.ReadTotalTimeoutConstant = 0;
    CommTimeouts.WriteTotalTimeoutMultiplier = 0;
    CommTimeouts.WriteTotalTimeoutConstant = 0;
    if (bIsSuccess)
    {
        bIsSuccess = SetCommTimeouts(m_hComm, &CommTimeouts);
    }

    DCB  dcb;
    if (bIsSuccess)
    {
        // ANSI to UNICODE.
        DWORD dwNum = MultiByteToWideChar(CP_ACP, 0, szDCBparam, -1, NULL, 0);
        wchar_t *pwText = new wchar_t[dwNum];
        if (!MultiByteToWideChar(CP_ACP, 0, szDCBparam, -1, pwText, dwNum))
        {
            bIsSuccess = TRUE;
        }
        // Gets the current serial configuration parameters, and construct a serial port DCB parameters.
        bIsSuccess = GetCommState(m_hComm, &dcb) && BuildCommDCB(WcharToChar(pwText).c_str(), &dcb);
        // Open RTS flow control.
        dcb.fRtsControl = RTS_CONTROL_ENABLE;
        delete[] pwText;
    }

    if (bIsSuccess)
    {
        // Use the DCB parameter configure a serial port state.
        bIsSuccess = SetCommState(m_hComm, &dcb);
    }

    // Empty the serial buffer.
    PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);

    // Leaving the critical secton.
    LeaveCriticalSection(&m_csCommunicationSync);
    return bIsSuccess == TRUE;
}

bool CSerialPort::InitPort(UINT portNo, const LPDCB& plDCB)
{
    if (!openPort(portNo))
    {
        return false;
    }
    EnterCriticalSection(&m_csCommunicationSync);
    if (!SetCommState(m_hComm, plDCB))
    {
        return false;
    }
    PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);
    LeaveCriticalSection(&m_csCommunicationSync);
    return true;
}

void CSerialPort::ClosePort()
{
    // If there is a serial port is open, close it.
    if (m_hComm != INVALID_HANDLE_VALUE)
    {
        CloseHandle(m_hComm);
        m_hComm = INVALID_HANDLE_VALUE;
    }
}

bool CSerialPort::openPort(UINT portNo)
{
    EnterCriticalSection(&m_csCommunicationSync);

    char szPort[50];
    sprintf_s(szPort, "COM%d", portNo);

    // Open the specified serial port.
    m_hComm = CreateFileA(szPort, // COMx.
        GENERIC_READ | GENERIC_WRITE,
        0,                        // Sharing(共享) mode, 0 means no sharing.
        NULL,                     // Security settings, general use NULL.
        OPEN_EXISTING,            // The parameters of said equipment must exist, or create failure.
        0,
        0);

    // If you open failure, release resources and return
    if (m_hComm == INVALID_HANDLE_VALUE)
    {
        LeaveCriticalSection(&m_csCommunicationSync);
        return false;
    }

    LeaveCriticalSection(&m_csCommunicationSync);
    return true;
}

bool CSerialPort::OpenListenThread()
{
    if (m_hListenThread != INVALID_HANDLE_VALUE)
    {
        return false;
    }

    s_bExit = false;
    UINT threadId;
    // Open the listen thread.
    m_hListenThread = (HANDLE)_beginthreadex(NULL, 0, ListenThread, this, 0, &threadId);
    if (!m_hListenThread)
    {
        return false;
    }
    // Set the priority(优先级) of a thread, is higher than common threads.
    if (!SetThreadPriority(m_hListenThread, THREAD_PRIORITY_ABOVE_NORMAL))
    {
        return false;
    }

    return true;
}

bool CSerialPort::CloseListenTread()
{
    if (m_hListenThread != INVALID_HANDLE_VALUE)
    {
        s_bExit = true;
        Sleep(10);
        CloseHandle(m_hListenThread);
        m_hListenThread = INVALID_HANDLE_VALUE;
    }
    return true;
}

UINT CSerialPort::GetBytesInCOM()
{
    DWORD dwError = 0; // Error code
    COMSTAT comstat;   // To record(记录) the status of communication equipment(通信设备).
    memset(&comstat, 0, sizeof(COMSTAT));

    UINT BytesInQue = 0;
    // Before calling ReadFile() and WriteFile(), through this functon clears the legacy before mistakes.
    if (ClearCommError(m_hComm, &dwError, &comstat))
    {
        BytesInQue = comstat.cbInQue; // Get the number of bytes in the input buffer.
    }

    return BytesInQue;
}

UINT WINAPI CSerialPort::ListenThread(void* pParam)
{
    // Get this pointer.
    CSerialPort *pSerialPort = reinterpret_cast<CSerialPort*>(pParam);

    // Serial data read polling mode.
    while (!pSerialPort->s_bExit)
    {
        UINT BytesInQue = pSerialPort->GetBytesInCOM();
        // If there is no data in the serial input buffer, the query again have a rest.
        if (BytesInQue == 0)
        {
            Sleep(SLEEP_TIME_INTERVAL);
            continue;
        }

        // Read and display
        char cRecved = 0x00;
        do
        {
            cRecved = 0x00;
            if (pSerialPort->ReadChar(cRecved) == true)
            {
                std::string tmpRecved{};
                tmpRecved.push_back(cRecved);
                g_IOBuf.PushBack(tmpRecved); // display
                if (tmpRecved[0] == '#' || tmpRecved[0] == u8'#')
                {
                    ++g_sendNext;
                }
                continue;
            }
        } while (--BytesInQue);
    }

    return 0;
}

bool CSerialPort::ReadChar(char &cRecved)
{
    BOOL  bResult = TRUE;
    DWORD BytesRead = 0;
    if (m_hComm == INVALID_HANDLE_VALUE)
    {
        return false;
    }

    EnterCriticalSection(&m_csCommunicationSync);

    // A byte of data read from the buffer.
    bResult = ReadFile(m_hComm, &cRecved, 1, &BytesRead, NULL);
    if ((!bResult))
    {
        // Empty the serial buffer.
        PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_RXABORT);
        LeaveCriticalSection(&m_csCommunicationSync);
        return false;
    }

    LeaveCriticalSection(&m_csCommunicationSync);
    return (BytesRead == 1);
}

bool CSerialPort::WriteData(unsigned char* pData, unsigned int length)
{
    BOOL   bResult = TRUE;
    DWORD  BytesToSend = 0;
    if (m_hComm == INVALID_HANDLE_VALUE)
    {
        return false;
    }

    EnterCriticalSection(&m_csCommunicationSync);

    // To write specifiled amount of data buffer.
    bResult = WriteFile(m_hComm, pData, length, &BytesToSend, NULL);
    if (!bResult)
    {
        // Empty the serial buffer.
        PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_RXABORT);
        LeaveCriticalSection(&m_csCommunicationSync);

        return false;
    }
    LeaveCriticalSection(&m_csCommunicationSync);
    return true;
}

void CSerialPort::InitSerialPortData()
{
    ClosePort();
}
