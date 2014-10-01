//
// MainPage.xaml.cpp
// Implementation of the MainPage class.
//

#include "pch.h"
#include "MainPage.xaml.h"
#include "Audio.h"
#include "BeatPoint.h"
#include "SequencerConfiguration.h"
#include "Sequencer.h"
using namespace AudioPlayground;


using namespace Platform;
using Platform::COMException;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;
using namespace Windows::UI::Input;
using namespace SequencerLib;

// The Blank Page item template is documented at http://go.microsoft.com/fwlink/?LinkId=234238

MainPage::MainPage()
{
	InitializeComponent();
	HRESULT hr = XAudio2Create(&pXAudio2);

	if (FAILED(hr))
		ref new COMException(hr, "XAudio2Create failure");

	// Create a mastering voice
	hr = pXAudio2->CreateMasteringVoice(&pMasteringVoice);

	if (FAILED(hr))
		ref new COMException(hr, "CreateMasteringVoice failure");
	SequencerFactory factory;
	SequencerObject = factory.Create(new SequencerConfiguration(2, 4, 1, 200), pXAudio2.Get());

	


}
/// <summary>
/// Invoked when this page is about to be displayed in a Frame.
/// </summary>
/// <param name="e">Event data that describes how this page was reached.  The Parameter
/// property is typically used to configure the page.</param>
void MainPage::OnNavigatedTo(NavigationEventArgs^ e)
{
	(void)e;	// Unused parameter
}

void AudioPlayground::MainPage::Canvas_PointerPressed_1(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e)
{
	int dataPoint = SequencerObject->Control->AddBeat(SequencerObject->Sound->GetBufferReadFromSourceVoice());

	PointerPoint^ p = e->GetCurrentPoint(RectCanvas);
	int x = p->RawPosition.X;
	int y = p->RawPosition.Y;
	BeatPoint bp = BeatPoint(x, y, dataPoint);
	sequenceOfBeats.AddPoint(bp);
}


void AudioPlayground::MainPage::Canvas_PointerMoved_1(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e)
{

}

int AudioPlayground::MainPage::GetTone(double pointer, double min, double max, double maxPointer)
{
	return (int)(min + ((max - min)*(pointer / maxPointer)));
}

