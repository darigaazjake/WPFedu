/*------------------------------------------------------
 * http://spark-debris.blogspot.jp/2015/06/cwpf-mvvm.html
 * C++でWPFするお手本
 *
 *------------------------------------------------------*/


#include "stdafx.h"

using System::STAThreadAttribute;
using System::String;
using System::ComponentModel::INotifyPropertyChanged;
using System::ComponentModel::PropertyChangedEventArgs;
using System::ComponentModel::PropertyChangedEventHandler;
using System::Windows::Application;
using System::Windows::TextAlignment;
using System::Windows::Thickness;
using System::Windows::Window;
using System::Windows::Controls::Border;
using System::Windows::Controls::Slider;
using System::Windows::Controls::StackPanel;
using System::Windows::Controls::TextBlock;
using System::Windows::Media::Brushes;

// データモデル
class DataModel {
public:

	DataModel()
		: _value(3.0) {
	}

	double GetValue() const {
		return _value;
	}

	bool SetValue(double value) {
		if (_value == value) {
			return false;
		}
		_value = value;
		return true;
	}

private:
	double _value;
};

// ビューモデル
ref class ViewModel : public INotifyPropertyChanged {
public:

	virtual event PropertyChangedEventHandler ^PropertyChanged;

	property double Value {

		double get() {
			return _dataModel->GetValue();
		}

		void set(double value) {
			if (_dataModel->SetValue(value)) {
				PropertyChanged(this, gcnew PropertyChangedEventArgs("Value"));
			}
		}

	}

	ViewModel()
		: _dataModel(new DataModel())
	{
	}

	~ViewModel() {
		this->!ViewModel();
	}

	!ViewModel() {
		delete _dataModel;
	}

private:
	DataModel *_dataModel;
};

// ビュー
ref class View : public Window {
public:
	View()
		: Window() {
		DataContext = gcnew ViewModel();
		Background = Brushes::Silver;
		Width = 640;
		Height = 200;

		TextBlock ^valueLabelTextBlock = gcnew TextBlock();
		valueLabelTextBlock->Margin = Thickness(16.0, 8.0, 16.0, 4.0);
		valueLabelTextBlock->Text = "Value:";

		Slider ^valueSlider = gcnew Slider();
		valueSlider->Margin = Thickness(32.0, 4.0, 32.0, 4.0);
		valueSlider->Minimum = 0.0;
		valueSlider->Maximum = 10.0;
		valueSlider->SmallChange = 0.1;
		valueSlider->LargeChange = 1.0;
		valueSlider->SetBinding(Slider::ValueProperty, "Value");

		TextBlock ^valueTextBlock = gcnew TextBlock();
		valueTextBlock->Margin = Thickness(32.0, 4.0, 32.0, 8.0);
		valueTextBlock->TextAlignment = TextAlignment::Center;
		valueTextBlock->SetBinding(TextBlock::TextProperty, "Value");

		StackPanel ^stackPanel = gcnew StackPanel();
		stackPanel->Margin = Thickness(8.0);
		stackPanel->Children->Add(valueLabelTextBlock);
		stackPanel->Children->Add(valueSlider);
		stackPanel->Children->Add(valueTextBlock);

		Border ^border = gcnew Border();
		border->Margin = Thickness(16.0);
		border->Background = Brushes::LightGray;
		border->BorderBrush = Brushes::Aquamarine;
		border->SetBinding(Border::BorderThicknessProperty, "Value");
		border->SetBinding(Border::CornerRadiusProperty, "Value");
		border->Child = stackPanel;

		Content = border;
	}
};

[STAThread]
int main(array<System::String ^> ^args)
{
	Application ^application = gcnew Application();
	application->Run(gcnew View());
	return 0;
}
