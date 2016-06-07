#pragma once

namespace AppKeyHandler
{
	void Init();
	void Run();
	bool IsEnabledRun();
	void Continue();
	void Stop();

	void KeyPressed(unsigned );
}