#ifndef TEXTENTRYDIALOG_H
#define TEXTENTRYDIALOG_H

#include <QDialog>

namespace Ui {
	class TextEntryDialog;
}

class TextEntryDialog : public QDialog
{
	Q_OBJECT

public:
	explicit TextEntryDialog(QWidget *parent = nullptr);
	~TextEntryDialog();

	//! get the entered text in the dialog
	std::string getText();

	//! set the initial text to be displayed in the dialog
	void setInitialText(const std::string& initialTxt);

private slots:
	//! handle clicking ok in the dialog
	void on_buttonBox_accepted();

private:
	Ui::TextEntryDialog *ui;

	//! enetered text
	std::string m_txtEntered;
};

#endif // TEXTENTRYDIALOG_H
