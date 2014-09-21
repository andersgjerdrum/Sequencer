//
// MainPage.xaml.h
// Declaration of the MainPage class.
//

#pragma once
#include "BeatPoint.h"
#include "MainPage.g.h"
#include "Audio.h"
using namespace Windows::UI::Xaml;
using namespace SequencerLib;
#include "MainPage.g.h"

namespace AudioPlayground
{
	/// <summary>
	/// An empty page that can be used on its own or navigated to within a Frame.
	/// </summary>
	public ref class MainPage sealed
	{

	private:
		//Do shared pointer
		Audio* Oscillator1;
		int Stopwatch;
		ISequencer * SequencerObject;
		BeatPoints sequenceOfBeats;
		Microsoft::WRL::ComPtr<IXAudio2> pXAudio2;
		IXAudio2MasteringVoice * pMasteringVoice;
	public:
		MainPage();

	protected:
		virtual void OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e) override;

	private:
		void Dummy(int sequenceId);
		void Canvas_PointerPressed_1(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e);
		void Canvas_PointerMoved_1(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e);
		int GetTone(double pointer, double min, double max, double maxPointer);
		void AudioPlayground::MainPage::DispatcherTimer_Tick(Platform::Object^ sender, Platform::Object^ e);
	};
}
