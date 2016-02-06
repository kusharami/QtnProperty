/*
   Copyright (c) 2012-1015 Alex Zhondin <qtinuum.team@gmail.com>

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#include "PropertyDelegateFloat.h"
#include "../../../Core/Core/PropertyFloat.h"
#include "../PropertyDelegateFactory.h"
#include "../PropertyEditorHandler.h"

#include "Utils/DoubleSpinBox.h"

#include <QLocale>

class QtnPropertyFloatSpinBoxHandler: public QtnPropertyEditorHandler<QtnPropertyFloatBase, QDoubleSpinBox>
{
public:
    QtnPropertyFloatSpinBoxHandler(QtnPropertyFloatBase& property, QDoubleSpinBox& editor)
        : QtnPropertyEditorHandlerType(property, editor)
    {
        if (!property.isEditableByUser())
            editor.setReadOnly(true);

        editor.setRange(property.minValue(), property.maxValue());
        editor.setSingleStep(property.stepValue());
		editor.setDecimals(6);

        updateEditor();

		editor.setKeyboardTracking(false);
		QObject::connect(&editor,
		static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
						 this,
						 &QtnPropertyFloatSpinBoxHandler::onValueChanged);
    }

private:
    void updateEditor() override
    {
		editor().setValue(property());
    }

	void onValueChanged(double value)
    {
		property() = static_cast<float>(value);
    }
};

static bool regFloatDelegate = QtnPropertyDelegateFactory::staticInstance()
                                .registerDelegateDefault(&QtnPropertyFloatBase::staticMetaObject
                                , &qtnCreateDelegate<QtnPropertyDelegateFloat, QtnPropertyFloatBase>
                                , "SpinBox");

QtnPropertyDelegateFloat::QtnPropertyDelegateFloat(QtnPropertyFloatBase &owner)
	: QtnPropertyDelegateTyped<QtnPropertyFloatBase>(owner)
	, percent_suffix(false)
{
}

QWidget* QtnPropertyDelegateFloat::createValueEditorImpl(QWidget* parent, const QRect& rect, QtnInplaceInfo* inplaceInfo)
{
	auto spinBox = new QtnDoubleSpinBox(parent);
	spinBox->setHavePercentSuffix(percent_suffix);
    spinBox->setGeometry(rect);

    new QtnPropertyFloatSpinBoxHandler(owner(), *spinBox);

    if (inplaceInfo)
    {
        spinBox->selectAll();
    }

    return spinBox;
}

bool QtnPropertyDelegateFloat::propertyValueToStr(QString& strValue) const
{
	strValue = QLocale().toString(owner().value(), 'g', 6);
	return true;
}
