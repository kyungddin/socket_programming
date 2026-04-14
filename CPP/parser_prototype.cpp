bool c::Parse(const char* pszText, int iTextLen)
{
    Clear();

    bool portMode = false;
    int iStartPos = 0;

    for (int i = 0; i < iTextLen; i++)
    {
        if (pszText[i] == ':' && portMode == false) // Step 1: protocol
        {
            if (i + 2 >= iTextLen || pszText[i + 1] != '/' || pszText[i + 2] != '/')
                return -1;

            if (i == 0)
                return -1;

            m_strProtocol.append(pszText, i);

            if (m_strProtocol == "https") m_iPort = 443;

            i += 2; 
            iStartPos = i + 1;
            portMode = true;
        }


        else if (pszText[i] == ':' && portMode) // Step 2: port
        {
            if (i == iStartPos)
                return -1;

            m_strHost.append(pszText + iStartPos, i - iStartPos);
            iStartPos = i + 1;
        }
        
        else if (pszText[i] == '/')  // Step 3: path
        {
            if (!portMode)
                return -1;

            if (m_strHost.empty())
            {
                if (i == iStartPos)
                    return -1;

                m_strHost.append(pszText + iStartPos, i - iStartPos);
            }
            else
            {
                m_iPort = atoi(std::string(pszText + iStartPos, i - iStartPos).c_str());
            }

            m_strPath.append(pszText + i, iTextLen - i);

            break;
        }
    }

    if (!portMode)
        return -1;

    if (m_strHost.empty())
    {
        if (iTextLen == iStartPos)
            return -1;

        m_strHost.append(pszText + iStartPos, iTextLen - iStartPos);
    }
    else
    {
        m_iPort = atoi(std::string(pszText + iStartPos, iTextLen - iStartPos).c_str());
    }

    return 0;
}