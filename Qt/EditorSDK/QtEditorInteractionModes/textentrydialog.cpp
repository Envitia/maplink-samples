#include "textentrydialog.h"
#include "ui_textentrydialog.h"

TextEntryDialog::TextEntryDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::TextEntryDialog)
{
	ui->setupUi(this);
}

TextEntryDialog::~TextEntryDialog()
{
	delete ui;
}

void TextEntryDialog::on_buttonBox_accepted()
{
	m_txtEntered = ui->lineEdit_textentry->text().toUtf8().data();
}

std::string TextEntryDialog::getText()
{
	return m_txtEntered;
}

void TextEntryDialog::setInitialText(const std::string& initialTxt)
{
	if (!initialTxt.empty())
	{
		ui->lineEdit_textentry->setText(initialTxt.c_str());
	}
}