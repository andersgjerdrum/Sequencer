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

    freqSlider->Value = 69;    // ie, 440 Hz
    VolSlider->Value = 2;
    
	submitButton->Visibility = Windows::UI::Xaml::Visibility::Visible;

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


void MainPage::OnSubmitButtonClick(Object^ sender, RoutedEventArgs^ args)
{
	
	return;
}


void Xaudio2Test::MainPage::Canvas_PointerPressed_1(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e)
{
	errorText->Text = L"Pointer pressed!";
}


void Xaudio2Test::MainPage::freqSlider_ValueChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e)
{

	if(Oscillator1 != nullptr && Oscillator2 != nullptr)
	{
		int noteNum = (int)e->NewValue;
		double freq = 440 *pow(2,(noteNum - 69) /12);
		Oscillator1->SetFrequency((float)freq);
		 freq = 110 *pow(2,(noteNum - 69) /12);
		Oscillator2->SetFrequency((float)freq);
	}
}


void Xaudio2Test::MainPage::Slider_ValueChanged_1(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e)
{
	if(Oscillator1 != nullptr && Oscillator2 != nullptr)
	{
		Oscillator1->SetAmplitude((float)e->NewValue/100);
		Oscillator2->SetAmplitude((float)e->NewValue/100);
	}
}
