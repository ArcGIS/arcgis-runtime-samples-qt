// [WriteFile Name=SearchDictionarySymbolStyle, Category=Search]
// [Legal]
// Copyright 2016 Esri.

// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0

// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
// [Legal]

#ifndef SEARCHDICTIONARYSYMBOLSTYLE_H
#define SEARCHDICTIONARYSYMBOLSTYLE_H

namespace Esri
{
  namespace ArcGISRuntime
  {
    class DictionarySymbolStyle;
    class SymbolStyleSearchResultListModel;
  }
}

#include <QQuickItem>
#include <QUuid>
#include <QAbstractListModel>

class SearchDictionarySymbolStyle : public QQuickItem
{
    Q_OBJECT
    Q_ENUMS(FieldEnum)

    Q_PROPERTY(Esri::ArcGISRuntime::SymbolStyleSearchResultListModel* searchResultsListModel READ searchResultsListModel NOTIFY searchResultsListModelChanged)

public:
    explicit SearchDictionarySymbolStyle(QQuickItem *parent = 0);
    ~SearchDictionarySymbolStyle();

    enum class FieldEnum {
        FieldNames,
        FieldTags,
        FieldClasses,
        FieldCategories,
        FieldKeys
    };

    void componentComplete() Q_DECL_OVERRIDE;
    Q_INVOKABLE void search(const QStringList& namesSearchParam, const QStringList& tagsSearchParam,
                            const QStringList& classesSearchParam,const QStringList& categoriesSearchParam,
                            const QStringList& keysSearchParam);

    Esri::ArcGISRuntime::SymbolStyleSearchResultListModel* searchResultsListModel() const;

signals:
    void searchCompleted(int count);
    void searchResultsListModelChanged();

private:
    Esri::ArcGISRuntime::DictionarySymbolStyle* m_dictionarySymbolStyle;
    Esri::ArcGISRuntime::SymbolStyleSearchResultListModel* m_searchResults;
};

#endif // SEARCHDICTIONARYSYMBOLSTYLE_H
