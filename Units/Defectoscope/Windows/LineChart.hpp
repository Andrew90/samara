#pragma once


template<class T, int N>struct Line: LineViewer
	{
		typedef LineViewer Parent;
		T *owner;		
		DataViewer<typename T::sub_type, N> dataViewer;
		Line()
		{
			chart.items.get<BarSeries>().SetColorBarHandler(this, &Line::GetColorBar);
			cursor.SetMouseMoveHandler(this, &Line<T, N>::CursorDraw);
		}	
		bool GetColorBar(int offs, double &data, unsigned &color)
		{
			if(NULL != dataViewer.data && offs < dataViewer.count)
			{
				data = dataViewer.data[offs];
				color = ConstData::ZoneColor(dataViewer.status[offs]);
				return true;
			}
			return false;
		}
		
		bool CursorDraw(TMouseMove &l, VGraphics &g)	  
		{	
			if(owner->drawZones)
			{
				double valY = chart.items.get<BarSeries>().ValueY(offsetX);
				char *s = StatusText(dataViewer.status[offsetX]);
				wsprintf(label.buffer, L"<ff>Зона <ff0000>%d <ff>датчик <ff0000>%d <ff>смещение %d  величина %s   %S     "
					, 1 + owner->lastZone, 1 + N, offsetX, Wchar_from<double, 5>(valY)(), s);
			}
			else
			{
				label.buffer[0] = 0;
			}
			label.Draw(g());
			return true;
		}
	};