[HvApiTest] 

----2009.1.22 Thu ----LM-----
1. M: 修改了需求文档中有关mode1的使用说明

----2009.1.22 Thu ----HYL-----
1. static lib of MFC.

v1.1.0.5

----2009.1.21 Wed ----LM-----
1.A:   CDhTestDlg::AOIFreq(int Range, int Precision)

2.M:   void CDhTestDlg::v_AOI_Test_Thread0(LPVOID lpConText)
		{
		    ……
			if (pMe->dlg.m_nAoiLangeControl) {
				CRect rect;
				::GetClientRect(hWndView,&rect);
				x1=rect.left;
				y1=rect.top;
				x2=rect.right-16;
				y2=rect.bottom-16;			
			}else{
				sscanf(pMe->dlg.m_StartCoordinate, "%[^,]", str);
				x1 = atoi(str);
				sscanf(pMe->dlg.m_StartCoordinate, "%*[^,],%s", str);
				y1 = atoi(str);
				sscanf(pMe->dlg.m_EndCoordinate, "%[^,]", str);
				x2 = atoi(str)-16;
				sscanf(pMe->dlg.m_EndCoordinate, "%*[^,],%s", str);
				y2 = atoi(str)-16;
			}
			……
			int dx1 = pMe->AOIFreq((int)(x2-x1),pMe->dlg.m_AOI_Precision);
			int dy1 = pMe->AOIFreq((int)(y2-y1),pMe->dlg.m_AOI_Precision);
			pt1.x=int(x1)+dx1*(pMe->dlg.m_AOI_Precision);
			pt1.y=int(y1)+dy1*(pMe->dlg.m_AOI_Precision);	
			
			int dx2 = pMe->AOIFreq((int)(x2-pt1.x),pMe->dlg.m_AOI_Precision);
			int dy2 = pMe->AOIFreq((int)(y2-pt1.y),pMe->dlg.m_AOI_Precision);		
			pt2.x=16+pt1.x+dx2*(pMe->dlg.m_AOI_Precision);
			pt2.y=16+pt1.y+dy2*(pMe->dlg.m_AOI_Precision);
			……
		}

V: 1.1.0.4

----2009.1.15 Thu ----HYL-----
1. A:
        do{
			pt1.x=int(x1+(double)rand()/(RAND_MAX+1)*(x2-x1));
			pt1.y=int(y1+(double)rand()/(RAND_MAX+1)*(y2-y1));
			pt2.x=int(pt1.x+(double)rand()/(RAND_MAX+1)*(x2-pt1.x));
			pt2.y=int(pt1.y+(double)rand()/(RAND_MAX+1)*(y2-pt1.y));	
			
		}while(pt2.x==pt1.x || pt2.y==pt1.y);
2. M:
		pt1.x=int(x1+(double)rand()/(RAND_MAX+1)*(x2-x1));
		pt1.y=int(y1+(double)rand()/(RAND_MAX+1)*(y2-y1));
		pt2.x=int(4+pt1.x+(double)rand()/(RAND_MAX+1)*(x2-pt1.x));
		pt2.y=int(4+pt1.y+(double)rand()/(RAND_MAX+1)*(y2-pt1.y));	
3.M:

		x2=rect.right-64;
		y2=rect.bottom-64;
	}
	while(b)
	{ 
		pt1.x=int(x1+(double)rand()/(RAND_MAX+1)*(x2-x1));
		pt1.y=int(y1+(double)rand()/(RAND_MAX+1)*(y2-y1));
		pt2.x=int(64+pt1.x+(double)rand()/(RAND_MAX+1)*(x2-pt1.x));
		pt2.y=int(64+pt1.y+(double)rand()/(RAND_MAX+1)*(y2-pt1.y));	
		


v: 1.1.0.3

----2008.9.5 Fri ----HYL-----
1. A: ::CloseHandle(hExclusion);
      ::ReleaseDC(hWndView,hdc);

v: 1.1.0.2
