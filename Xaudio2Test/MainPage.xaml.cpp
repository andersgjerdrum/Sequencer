//
// MainPage.xaml.cpp
// Implementation of the MainPage class.
//

#include "pch.h"
#include "MainPage.xaml.h"
#include "Audio.h"

using namespace Xaudio2Test;

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
// The Blank Page item template is documented at http://go.microsoft.com/fwlink/?LinkId=234238

MainPage::MainPage()
{
	InitializeComponent();
	/// Create an IXAudio2 object
    HRESULT hr = XAudio2Create(&pXAudio2);

    if (FAILED(hr))
        ref new COMException(hr, "XAudio2Create failure");

    // Create a mastering voice
    hr = pXAudio2->CreateMasteringVoice(&pMasteringVoice);

    if (FAILED(hr))
        ref new COMException(hr, "CreateMasteringVoice failure");

	Oscillator1 = new Audio(pXAudio2.Get());
	Oscillator2 = new Audio(pXAudio2.Get());

}


/// <summary>
/// Invoked when this page is about to be displayed in a Frame.
/// </summary>
/// <param name="e">Event data that describes how this page was reached.  The Parameter
/// property is typically used to configure the page.</param>
void MainPage::OnNavigatedTo(NavigationEventArgs^ e)
{
	(void) e;	// Unused parameter
}



void Xaudio2Test::MainPage::Canvas_PointerPressed_1(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e)
{
	PointerPoint^ p = e->GetCurrentPoint(RectCanvas);
	int x = p->RawPosition.X;
	int y = p->RawPosition.Y;
	if(Oscillator1 != nullptr && Oscillator2 != nullptr)
	{
		int noteNum = 50;
		double freq = GetTone(x,110,440,RectCanvas->RenderSize.Width) *pow(2,(noteNum - 69) /12);
		Oscillator1->SetFrequency((float)freq);
		double freq2 = GetTone(y,110,440,RectCanvas->RenderSize.Height) *pow(2,(noteNum - 69) /12);
		Oscillator2->SetFrequency((float)freq);
		errorText->Text = "Pos:(" + x.ToString() + "," + y.ToString() + ")" + "Freq:(" + freq.ToString() + "," + freq2.ToString() + ")";
	}

}

void Xaudio2Test::MainPage::Canvas_PointerMoved_1(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e)
{
	PointerPoint^ p = e->GetCurrentPoint(RectCanvas);
	int x = p->RawPosition.X;
	int y = p->RawPosition.Y;
	if(Oscillator1 != nullptr && Oscillator2 != nullptr)
	{
		int noteNum = 50;
		double freq = GetTone(x,110,440,RectCanvas->RenderSize.Width) *pow(2,(noteNum - 69) /12);
		Oscillator1->SetFrequency((float)freq);
		double freq2 = GetTone(y,110,440,RectCanvas->RenderSize.Height) *pow(2,(noteNum - 69) /12);
		Oscillator2->SetFrequency((float)freq);
		errorText->Text = "Pos:(" + x.ToString() + "," + y.ToString() + ")" + "Freq:(" + freq.ToString() + "," + freq2.ToString() + ")";
	}
}

int Xaudio2Test::MainPage::GetTone(double pointer, double min, double max, double maxPointer)
{
	return (int)(min + ((min - max)*(pointer/maxPointer)));
}
