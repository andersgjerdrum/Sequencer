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
using namespace Windows::Storage;
using namespace Windows::Storage::Pickers;
using namespace Windows::Storage::Streams;
using namespace Windows::System::Threading;
using namespace Windows::UI::Input;
using namespace concurrency;
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



void AudioPlayground::MainPage::Button_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	OpenFile();
}

void MainPage::OpenFile()
{

	FileOpenPicker^ getAudioSample = ref new FileOpenPicker();

	getAudioSample->SuggestedStartLocation = PickerLocationId::MusicLibrary;
	getAudioSample->ViewMode = PickerViewMode::List;

	getAudioSample->FileTypeFilter->Clear();
	getAudioSample->FileTypeFilter->Append(".wma");
	getAudioSample->FileTypeFilter->Append(".wav");

	concurrency::cancellation_token_source m_tcs;

	auto m_pickFileTask = task<StorageFile^>(getAudioSample->PickSingleFileAsync(), m_tcs.get_token());

	m_pickFileTask.then([this](StorageFile^ fileHandle)
	{
		try
		{
			if (!fileHandle)
			{
				//No Audio:(
				return;
			}

			task<IRandomAccessStream^> fOpenStreamTask(fileHandle->OpenAsync(Windows::Storage::FileAccessMode::Read));


		
			fOpenStreamTask.then([this](IRandomAccessStream^ streamHandle)
			{
				SequencerFactory factory;
				SequencerObject = factory.Create(10, pXAudio2.Get(), streamHandle);
			});

		}
		catch (Platform::Exception^)
		{
			throw;
		}
	});

	return;
}

