//#include "stdafx.h"
#include "packetbuf.h"

//CRecvBuf *CRecvBuf::m_pInstance = 0;
template <> CRecvBuf *CSingle<CRecvBuf>::m_pInstance = 0;
// CRecvBuf *CSingle<CRecvBuf>::m_pInstance = 0;            // �ϰ汾��ʽ 

template <> CSendBuf *CSingle<CSendBuf>::m_pInstance = 0;
