//-----------------------------------------------------------------------------
//  (c) 2002 by Basler Vision Technologies
//  Section:  Vision Components
//  Project:  BCAM
//  $Header: BcamPropertyBag.cpp, 4, 11/25/02 7:00:32 PM, Happe, A.$
//-----------------------------------------------------------------------------
/**
  \file     BcamPropertyBag.cpp
 *
  \brief   Implementation of CPropertyBag, CIniFilePropertyBag, and CRegistryPropertyBag
  classes 
 *
 * The property bag classes are used to load / store a Bcam device's configuration 
 */
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "PropertyBag.h"


using namespace HVDevice;




//////////////////////////////////////////////////////////////////////////////////
///
///   Implementation of CPropertyBag
///
//////////////////////////////////////////////////////////////////////////////////

void CPropertyBag::WriteBool(const CString& name, const bool value)
{
  DWORD error = WriteProperty(name, value ? "true" : "false");
  if ( error != 0 )
    throw HVBaseException(error, "CPropertyBag::WriteBool()");
}

bool CPropertyBag::ReadBool(const CString& name)
{
  CString buffer;
  DWORD error;
  if ( error = ReadProperty(name, buffer) )
  {
    throw HVBaseException(error, "CPropertyBag::ReadBool()");
  }
  if ( buffer.CompareNoCase("false") == 0 )
    return false;
  else if ( buffer.CompareNoCase("true") == 0 )
    return true;
  else
    throw HVBaseException(STATUS_PARAMETER_INVALID, "CPropertyBag::ReadBool()");
}

void CPropertyBag::WriteLong(const CString& name, const long value)
{
  DWORD error;
  TCHAR buffer[256];
  _ltot(value, buffer, 10);
  if ( error = WriteProperty(name, buffer) )
    throw HVBaseException(error, "CPropertyBag::WriteLong()");
}

long CPropertyBag::ReadLong(const CString& name)
{
  CString buffer;
  DWORD error;
  if ( error = ReadProperty(name, buffer) )
  {
    throw HVBaseException(error, "CPropertyBag::ReadLong()");
  }
  long res;
  if ( ! _stscanf(buffer, "%ld", &res) )
    throw HVBaseException(STATUS_PARAMETER_INVALID, "CPropertyBag::ReadLong()");
  return res;
}

void CPropertyBag::WriteFloat(const CString& name, const float value)
{
  TCHAR buffer[256];
  _stprintf(buffer, "%f", value);
  DWORD error;
  if ( error = WriteProperty(name, buffer ))
    throw HVBaseException(error, "CPropertyBag::WriteFloat()");
}

float CPropertyBag::ReadFloat(const CString& name)
{
  CString buffer;
  DWORD error;
  if ( error = ReadProperty(name, buffer) ) 
  {
    throw HVBaseException(error, "CPropertyBag::ReadFloat()");
  }
  float res;
  if ( ! _stscanf(buffer, "%f", &res) )
    throw HVBaseException(STATUS_PARAMETER_INVALID, "CPropertyBag::ReadFloat()");
  return res;
}

void CPropertyBag::WriteString(const CString& name, const CString value)
{
  DWORD error;
  if ( error = WriteProperty(name, value) )
    throw HVBaseException(error, "CPropertyBag::WriteString()");
}

CString CPropertyBag::ReadString(const CString& name)
{
  CString buffer;
  DWORD error;
  if ( error = ReadProperty(name, buffer) ) 
    throw HVBaseException(error, "CPropertyBag::ReadFloat()");
  return buffer;
}


//////////////////////////////////////////////////////////////////////////////////
///
///   Implementation of CIniFilePropertyBag
///
//////////////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------------
// CIniFilePropertyBag::CIniFilePropertyBag(const CString& fileName, const CString& sectionName,const bool create) : m_strFileName(fileName), m_strSectionName(sectionName)
// Author: 
// Date: 04.09.2002
//------------------------------------------------------------------------------
/**
 * Protected constructor. To create a property bag, use one of the static factory methods
 *
 * \param     fileName name of the .ini file
 * \param     sectionName name of the .ini file's section the created bag is bound to
 * \param     create true: a new .ini file will be created, false: an existing one will be used
 */
//------------------------------------------------------------------------------
CIniFilePropertyBag::CIniFilePropertyBag(const CString& fileName, const CString& sectionName,const bool create) : 
m_strFileName(fileName), 
m_strSectionName(sectionName)
{
  bool exist = false;
  char lpszReturnBuffer[1024];
  
  // check if already an section named entryName exists
  if ( GetPrivateProfileSectionNames(lpszReturnBuffer, 1024, m_strFileName) )
  {
    char* p = lpszReturnBuffer;
    while ( *p != 0 )
    {
      if ( sectionName.CompareNoCase(p) == 0 )
      {
        exist = true;
        break;
      }
      while ( *p++);
    }
  } 
  if ( create )
  {
    if ( exist )
      throw HVBaseException( STATUS_INTERNAL_ERROR, "CIniFilePropertyBag::CIniFilePropertyBag()");
    // write an empty section for the BAG
    if ( ! WritePrivateProfileSection(m_strSectionName, "\0\0", m_strFileName) )
      throw HVBaseException( GetLastError(), "CIniFilePropertyBag::Create()" );
  }
  else
  {
    if ( ! exist )
      throw HVBaseException( STATUS_INTERNAL_ERROR, "CIniFilePropertyBag::CIniFilePropertyBag()");
  }
}

//------------------------------------------------------------------------------
// CPropertyBagPtr CIniFilePropertyBag::Create(const CString& fileName, const CString& bagName)
// Author: 
// Date: 04.09.2002
//------------------------------------------------------------------------------
/**
 *
 * 
 * Static factory method to create a new .ini file. An existing .ini file will be 
 * deleted.
 *
 * \param     fileName  name of the ini.file
 * \param     bagName name of the section the bag is bound to
 * \return    
 *
 * A property bag bound to a section named bagName
 * 
 */
//------------------------------------------------------------------------------

CPropertyBagPtr CIniFilePropertyBag::Create(const CString& fileName, const CString& bagName)
{
  DWORD numCharsCopied;
  char pszFilePath[MAX_PATH];
  LPTSTR *lpFilePart = NULL;

  // WritePrivateProfileString works relative to the windows directory if no fully qualified 
  // file name is used. So we will use SearchPath to work relative to the current directory

  numCharsCopied = SearchPath(
    NULL,                 // search path
    (LPCTSTR) fileName,   // file name
    NULL,                 // file extension
    MAX_PATH,             // size of buffer
    pszFilePath,          // found file name buffer
    lpFilePart );         // file component
  if( ! numCharsCopied )
  {
    // file doesn't exist, create a dummy one
    FILE *hFile = fopen((LPCTSTR) fileName, "w");
    if ( hFile == NULL )
      throw HVBaseException(ERROR_CANNOT_MAKE, "CIniFilePropertyBag::Create()");
    fclose(hFile);
    numCharsCopied = SearchPath(
      NULL,                 // search path
      (LPCTSTR) fileName,   // file name
      NULL,                 // file extension
      MAX_PATH,             // size of buffer
      pszFilePath,          // found file name buffer
      lpFilePart );         // file component
    if ( ! numCharsCopied )
      throw HVBaseException( GetLastError(), "CIniFilePropertyBag::Create()" );
  }
  else
  {
    // There already exists such an file, delete it
    DeleteFile(pszFilePath);
  }

  return CPropertyBagPtr(new CIniFilePropertyBag(pszFilePath, bagName, true));
}

//------------------------------------------------------------------------------
// CPropertyBagPtr CIniFilePropertyBag::Open(const CString& fileName, const CString& bagName)
// Author: 
// Date: 04.09.2002
//------------------------------------------------------------------------------
/**
 * Static factory method. The function returns a property bag which is bound to a specified 
 * section of an existing .ini file
 *
 * \param     fileName name of the .ini file
 * \param     bagName name of the section the bag is to be bound to
 *
 * \return A property bag bound smart pointer
 * 
 */
//------------------------------------------------------------------------------
CPropertyBagPtr CIniFilePropertyBag::Open(const CString& fileName, const CString& bagName)
{
  DWORD numCharsCopied;
  char pszFilePath[MAX_PATH];
  LPTSTR *lpFilePart = NULL;

  numCharsCopied = SearchPath(
    NULL,                 // search path
    (LPCTSTR) fileName,   // file name
    NULL,                 // file extension
    MAX_PATH,             // size of buffer
    pszFilePath,          // found file name buffer
    lpFilePart );         // file component
  if( ! numCharsCopied )
  {
      throw HVBaseException( GetLastError(), "CIniFilePropertyBag::Open()" );
  }

  return CPropertyBagPtr(new CIniFilePropertyBag(pszFilePath, bagName, false));
}


//------------------------------------------------------------------------------
// CPropertyBagPtr CIniFilePropertyBag::CreateBag(const CString& name)
// Author: 
// Date: 04.09.2002
//------------------------------------------------------------------------------
/**
 * Creates a new section in the .ini file and returns a property bag bound to it
 *
 * \param     name The section's name
 * \return    
 *
 * A property bag
 * 
 */
//------------------------------------------------------------------------------
CPropertyBagPtr CIniFilePropertyBag::CreateBag(const CString& name)
{
  return CPropertyBagPtr(new CIniFilePropertyBag(m_strFileName, name, true));
}

//------------------------------------------------------------------------------
// CPropertyBagPtr CIniFilePropertyBag::GetBag(const CString& name)
// Author: 
// Date: 04.09.2002
//------------------------------------------------------------------------------
/**
 * Returns a property bag bound to a given secition in the .ini file
 *
 * \param     name The section's name
 * \return    
 *
 * A property bag
 * 
 */
//------------------------------------------------------------------------------
CPropertyBagPtr CIniFilePropertyBag::GetBag(const CString& name)
{
  return CPropertyBagPtr(new CIniFilePropertyBag(m_strFileName, name, false));
}

//------------------------------------------------------------------------------
// DWORD CIniFilePropertyBag::WriteProperty(const CString& name, const CString& value)
// Author: 
// Date: 04.09.2002
//------------------------------------------------------------------------------
/**
 * Write a value to the section the bag is bound to
 *
 * \param     name  name of the value
 * \param     value value
 * \return    0 if the function succeeds, otherwise an error code
 *
 */
//------------------------------------------------------------------------------
DWORD CIniFilePropertyBag::WriteProperty(const CString& name, const CString& value)
{
  if ( ! WritePrivateProfileString(m_strSectionName, name, value, m_strFileName) )
    return GetLastError();
  return 0;
}

//------------------------------------------------------------------------------
// DWORD CIniFilePropertyBag::ReadProperty(const CString& name, CString& value)
// Author: 
// Date: 04.09.2002
//------------------------------------------------------------------------------
/**
 * Read a value from a section the bag is bound to.
 *
 * \param     name name of the property 
 * \param     value String which will receive the value
 * \return    
 *
 * 0 if the function succeeds, otherwise an error code
 * 
 * \see       <delete line if not used>
 * \todo      
 */
//------------------------------------------------------------------------------

DWORD CIniFilePropertyBag::ReadProperty(const CString& name, CString& value)
{
  TCHAR buffer[1024];
  if ( ! GetPrivateProfileString(m_strSectionName, name, "", buffer, 1024, m_strFileName) )
    return STATUS_INTERNAL_ERROR;
  value = buffer;
  return 0;
}


//////////////////////////////////////////////////////////////////////////////////
///
///   Implementation of CIniFilePropertyBag
///
//////////////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------------
// CRegistryPropertyBag::CRegistryPropertyBag(HKEY parent, const CString& bagName, bool create)
// Author: A.Happe
// Date: 06.09.2002
//------------------------------------------------------------------------------
/**
 * Constructor. It creates a property bag which is bound to a specified registry key
 *
 * \param     parent The parent registry key of the property bag
 * \param     bagName Name of the property bag's registry key (relative to the parent)
 * \param     create if set to true, a new bag will be created, otherwise an existing one will be returned
 */
//------------------------------------------------------------------------------
CRegistryPropertyBag::CRegistryPropertyBag(HKEY parent, const CString& bagName, bool create)
{
  // check if the bag already exists;
  LONG error = m_Key.Open(parent, bagName);
  bool exist = error == ERROR_SUCCESS;
  if ( create && exist )
  {
    // the bag already exists
    throw HVBaseException(STATUS_INTERNAL_ERROR, "CRegistryPropertyBag::CRegistryPropertyBag()");
  }
  if ( ! create && ! exist )
  {
    throw HVBaseException(STATUS_INTERNAL_ERROR, "CRegistryPropertyBag::CRegistryPropertyBag()");
  }
  if ( create )
  {
    error = m_Key.Create(parent, bagName);
    if ( error != ERROR_SUCCESS )
      throw HVBaseException(error, "CRegistryPropertyBag::CRegistryPropertyBag()");
  }
}


//------------------------------------------------------------------------------
// CPropertyBagPtr CRegistryPropertyBag::Open(const CString& bagName, HKEY hRoot /* = HKEY_CURRENT_USER */)
// Author: 
// Date: 06.09.2002
//------------------------------------------------------------------------------
/**
 * Opens an existing property bag
 *
 * \param     bagName Name of the registry key the bag is bound to
 * \param     hRoot handle to the parent key
 * \return    a smart pointer to the property bag
 */
//------------------------------------------------------------------------------
CPropertyBagPtr CRegistryPropertyBag::Open(const CString& bagName, HKEY hRoot /* = HKEY_CURRENT_USER */)
{
  return CPropertyBagPtr(new CRegistryPropertyBag(hRoot, bagName, false));
}

//------------------------------------------------------------------------------
// CPropertyBagPtr CRegistryPropertyBag::Create(const CString& bagName, HKEY hRoot /* = HKEY_CURRENT_USER */)
// Author: 
// Date: 06.09.2002
//------------------------------------------------------------------------------
/**
 * Create a new property bag. If the property bag already exists, it will be deleted!
 *
 * \param     bagName Name of the registry key the bag is bound to
 * \param     hRoot handle to the parent key
 * \return    a smart pointer to the created property bag
 *
 */
//------------------------------------------------------------------------------
CPropertyBagPtr CRegistryPropertyBag::Create(const CString& bagName, HKEY hRoot /* = HKEY_CURRENT_USER */)
{
  CRegKey key;
  LONG error = key.Create(hRoot, bagName );
  if ( error == ERROR_SUCCESS )
  {
    key.Close();
    // bag already exist. Delete it
    key.Attach(hRoot);
    key.RecurseDeleteKey(bagName);
  }
  return CPropertyBagPtr(new CRegistryPropertyBag(hRoot, bagName, true));
}

//------------------------------------------------------------------------------
// CPropertyBagPtr CRegistryPropertyBag::CreateBag(const CString& Name)
// Author: 
// Date: 06.09.2002
//------------------------------------------------------------------------------
/**
 * Create a new property bag as a child of the current one. If the bag already
 * exists, an exception will be thrown
 *
 * \param     Name name of the registry key representing the bag
 * \return    a smart pointer to the created bag.
 *
 */
//------------------------------------------------------------------------------
CPropertyBagPtr CRegistryPropertyBag::CreateBag(const CString& Name)
{
  return CPropertyBagPtr(new CRegistryPropertyBag(m_Key, Name, true));
}


//------------------------------------------------------------------------------
// CPropertyBagPtr CRegistryPropertyBag::GetBag(const CString& Name)
// Author: 
// Date: 06.09.2002
//------------------------------------------------------------------------------
/**
 * Opens a new property bag which is a child of the current one. I the bag doesn't 
 * exist, an exception will be thrown.
 *
 * \param     Name name of the registry key representing the child bag
 * \return    a smart pointer to the bag
 *
 */
//------------------------------------------------------------------------------
CPropertyBagPtr CRegistryPropertyBag::GetBag(const CString& Name)
{
  return CPropertyBagPtr(new CRegistryPropertyBag(m_Key, Name, false));
}

//------------------------------------------------------------------------------
// DWORD CRegistryPropertyBag::WriteProperty(const CString& name, const CString& value)
// Author: 
// Date: 06.09.2002
//------------------------------------------------------------------------------
/**
 * Write a value to the registry key the bag is bound to
 *
 * \param     name name of the value
 * \param     value the value
 * \return    0 if the function succeeds, otherwise an error code
 *
 */
//------------------------------------------------------------------------------
DWORD CRegistryPropertyBag::WriteProperty(const CString& name, const CString& value)
{
  return m_Key.SetValue(value, name);
}

//------------------------------------------------------------------------------
// DWORD CRegistryPropertyBag::ReadProperty(const CString& name, CString& value)
// Author: 
// Date: 06.09.2002
//------------------------------------------------------------------------------
/**
 * Read a value from the registry key the bag is bound to
 *
 * \param     name name of the value
 * \param     value the value
 * \return    0 if the function succeeds, otherwise an error code
 *
 */
//------------------------------------------------------------------------------
DWORD CRegistryPropertyBag::ReadProperty(const CString& name, CString& value)
{
  TCHAR buffer[1024];
  ULONG size = 1024;
  LONG error = m_Key.QueryValue( buffer, name, &size);
  if ( error == ERROR_SUCCESS )
    value = buffer;
  if ( error == ERROR_FILE_NOT_FOUND )
    error = STATUS_INTERNAL_ERROR;
  return error;
}
