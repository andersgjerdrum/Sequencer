//
// MainPage.xaml.h
// Declaration of the MainPage class.
//

#pragma once

#include "MainPage.g.h"
#include "Audio.h"
#include "Sequencer.h"
using namespace Windows::UI::Xaml;

namespace Xaudio2Test
{
	/// <summary>
	/// An empty page that can be used on its own or navigated to within a Frame.
	/// </summary>
	public ref class MainPage sealed
	{

	 private:
		DispatcherTimer^ SequenceTimer;
		Sequencer^ sequencer;
		Audio* Oscillator1;
		Audio* Oscillator2;
		BeatPoints sequenceOfBeats;
		Microsoft::WRL::ComPtr<IXAudio2> pXAudio2;
		IXAudio2MasteringVoice * pMasteringVoice;
	public:
		MainPage();

	protected:
		virtual void OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e) override;

	private:
		void Xaudio2Test::MainPage::Dummy(int sequenceId);
		void Canvas_PointerPressed_1(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e);
		void Canvas_PointerMoved_1(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e);
		int GetTone(double pointer, double min, double max, double maxPointer);
		void Xaudio2Test::MainPage::DispatcherTimer_Tick(Platform::Object^ sender, Platform::Object^ e);
	};
}
