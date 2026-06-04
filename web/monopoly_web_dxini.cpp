#include "PC3DHdr.h"
#include "DXINI.h"

char g_szDXINIPath[MAX_PATH] = "/monopoly-user/DirectX.ini";
DXINI_SelectionDialogInfo *g_pDlgInfo = NULL;

void DXINI_GetFilePath()
{
  strcpy(g_szDXINIPath, "/monopoly-user/DirectX.ini");
}

BOOL DXINI_FileExists()
{
  if (!DXINI_PathIsValid())
    DXINI_GetFilePath();
  return TRUE;
}

BOOL DXINI_ReadGUID(LPCTSTR, LPCTSTR, GUID& guid)
{
  ZeroMemory(&guid, sizeof(guid));
  return TRUE;
}

BOOL DXINI_WriteGUID(LPCTSTR, LPCTSTR, const GUID&)
{
  return TRUE;
}

BOOL DXINI_ReadRes(LPCTSTR, LPCTSTR, DWORD& dwWidth, DWORD& dwHeight, DWORD& dwBitDepth)
{
  dwWidth = 800;
  dwHeight = 600;
  dwBitDepth = 32;
  return TRUE;
}

BOOL DXINI_WriteRes(LPCTSTR, LPCTSTR, DWORD, DWORD, DWORD)
{
  return TRUE;
}

int DXINI_GetDriverList(DXINI_DriverInfo* aDrivers, int nMaxDrivers)
{
  if (aDrivers == NULL || nMaxDrivers <= 0)
    return 0;

  ZeroMemory(&aDrivers[0], sizeof(aDrivers[0]));
  strcpy(aDrivers[0].driver.szName, "webgl");
  strcpy(aDrivers[0].driver.szDesc, "Monopoly WebGL display");
  strcpy(aDrivers[0].a3DDevices[0].szName, "webgl2");
  strcpy(aDrivers[0].a3DDevices[0].szDesc, "Browser WebGL2 renderer");
  aDrivers[0].nNum3DDevices = 1;
  return 1;
}

BOOL DXINI_ChooseDriver(int, int, BOOL bWindowed)
{
  DXINI_SetSelectedDriver(0);
  DXINI_SetSelected3DDevice(0);
  DXINI_SetWindowedMode(TRUE);
  DXINI_SetUsingResolution(FALSE);
  DXINI_SetSelectedResolution(800, 600, 32);

  if (g_pDlgInfo != NULL) {
    g_pDlgInfo->nSelectedDriver = 0;
    g_pDlgInfo->nSelected3DDevice = 0;
    g_pDlgInfo->bWindowedMode = bWindowed;
  }
  return TRUE;
}

BOOL DXINI_CheckForChooseSettings()
{
  return FALSE;
}

BOOL DXINI_WriteD3DDeviceInfo(const D3DDEVICEDESC&, LPSTR, LPSTR, LPCTSTR)
{
  return TRUE;
}

BOOL DXINI_InitDrvSelDlgTemplate(WORD*)
{
  return TRUE;
}

BOOL DXINI_InitDrvSelDlg(HWND)
{
  return TRUE;
}

BOOL DXINI_CloseDrvSelDlg(HWND)
{
  if (g_pDlgInfo != NULL) {
    g_pDlgInfo->nSelectedDriver = 0;
    g_pDlgInfo->nSelected3DDevice = 0;
    g_pDlgInfo->bWindowedMode = TRUE;
  }
  return TRUE;
}

BOOL DXINI_DrvChangedDrvSelDlg(HWND)
{
  return TRUE;
}

LRESULT APIENTRY DrvSelDlgProc(HWND hWnd, UINT uiMsg, WPARAM, LPARAM)
{
  if (uiMsg == WM_INITDIALOG || uiMsg == WM_COMMAND)
    DXINI_CloseDrvSelDlg(hWnd);
  return TRUE;
}
