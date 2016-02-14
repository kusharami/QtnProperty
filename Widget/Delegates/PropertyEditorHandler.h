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

#ifndef PROPERTY_EDITOR_HANDLER_H
#define PROPERTY_EDITOR_HANDLER_H

#include "../PropertyWidgetAPI.h"
#include "../../Core/Property.h"
#include <QWidget>
#include <QEvent>

class QTN_PW_EXPORT QtnPropertyEditorHandlerBase: public QObject
{
protected:
    QtnPropertyEditorHandlerBase(QtnProperty& property, QWidget& editor);

    virtual QtnProperty& propertyBase() = 0;
    virtual QWidget& editorBase()  = 0;
    virtual void updateEditor() = 0;
	virtual void revertInput();

	virtual bool eventFilter(QObject *obj, QEvent *event) override;

private:
    void onObjectDestroyed(QObject* object);
    void onPropertyDidChange(const QtnPropertyBase* changedProperty, const QtnPropertyBase* firedProperty, QtnPropertyChangeReason reason);
};

template <typename PropertyClass, typename PropertyEditorClass>
class QtnPropertyEditorHandler: public QtnPropertyEditorHandlerBase
{
protected:
    typedef QtnPropertyEditorHandler<PropertyClass, PropertyEditorClass> QtnPropertyEditorHandlerType;

    QtnPropertyEditorHandler(PropertyClass& property, PropertyEditorClass& editor)
        : QtnPropertyEditorHandlerBase(property, editor),
          m_property(property),
          m_editor(editor)
    {
    }

	PropertyClass &property() { return m_property;  }
	PropertyEditorClass &editor() { return m_editor; }

	virtual QtnProperty &propertyBase() override { return property(); }
	virtual QWidget &editorBase() override {return editor(); }

private:
    PropertyClass& m_property;
    PropertyEditorClass& m_editor;
};

template <typename PropertyClass, typename PropertyEditorClass>
class QtnPropertyEditorBttnHandler
	: public QtnPropertyEditorHandler<PropertyClass, PropertyEditorClass>
{
private:
	typedef QtnPropertyEditorHandler<PropertyClass, PropertyEditorClass> Inherited;
protected:
	typedef QtnPropertyEditorBttnHandler QtnPropertyEditorHandlerType;

	QtnPropertyEditorBttnHandler(PropertyClass& property, PropertyEditorClass& editor)
		: Inherited(property, editor)
	{

	}

	virtual void onToolButtonClick() = 0;
	virtual bool eventFilter(QObject *obj, QEvent *event) override
	{
		switch (event->type())
		{
			case QEvent::MouseButtonDblClick:
				onToolButtonClick();
				return true;

			default:
				break;
		}

		return Inherited::eventFilter(obj, event);
	}
};


#endif // PROPERTY_EDITOR_HANDLER_H
