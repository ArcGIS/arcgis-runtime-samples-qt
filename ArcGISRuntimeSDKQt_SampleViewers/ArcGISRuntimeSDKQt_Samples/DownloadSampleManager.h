// [Legal]
// Copyright 2022 Esri.

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

#ifndef DOWNLOADSAMPLEMANAGER_H
#define DOWNLOADSAMPLEMANAGER_H

#include "SampleManager.h"

/*!
 * \brief The DownloadSampleManager class is an abstract base class for the
 * QMLSampleManager and CPPSampleManager. It deals with creating a unified
 * interface for the DataDownloader QML system to download data via portals
 * without exposing the two different QML/C++ interfaces for PortalItems.
 *
 * The QML and CPP Sample managers respectively overload the virtual functions,
 * and emit the appropriate portal* signals when the portal or a portal item
 * has triggered a change.
 */
class DownloadSampleManager : public SampleManager
{
  Q_PROPERTY(QString api READ api CONSTANT)
  Q_OBJECT

public:
  explicit DownloadSampleManager(QObject* parent = nullptr);

  ~DownloadSampleManager() override;

  void init() override;

  Q_INVOKABLE void downloadData(const QString& itemId, const QString& outputPath);

signals:
  /*
   * The following functions are consumed by DataDownlaoder to interact with the
   * SampleManger to consume requested data.
   */

  void unzipFile(const QString& filePath, const QString& extractPath);

  void doneDownloadingPortalItem();

  void downloadDataFailed(const QString& itemId);

  /*
   * The following signals are fired by subclasses to feed back into the
   * SampleManager parent when a portal has triggered appropriately.
   */

  /*!
   * \brief signal emitted by subclass when portal has loaded.
   * \param success true if succeeded, false if there was an issue.
   */
  void portalDoneLoading(bool success);


  /*!
   * \brief signal emitted by subclass when portal item has loaded.
   * \param success true if succeeded, false if there was an issue.
   * \param itemId ID that has come out of the portal that loaded.
   * \param folderName optional subfolde -name for the portal item to be
   *        extracted to. Nominally this is given if the portal item type is of
   *        type "CodeSample", otherwise is blank.
   */
  void portalItemDoneLoading(bool success,
                             const QString& itemId,
                             const QString& folderName);

  /*!
   * \brief signal emitted by subclass when the portal item has loaded.
   * \param itemId id of the portal item with a fetch update.
   * \param percentage [0-100] value of percentage downloaded so far.
   */
  void portalItemFetchDataProgress(const QString& itemId, int percentage);

  /*!
   * \brief signal emitted by subclass when the portal item has finished
   *        downloading.
   * \param itemId id of the portal item that has finsihed loading.
   * \param success true if the download finished successfully, otherwise
   *        failed.
   */
  void portalItemFetchDataCompleted(const QString& itemId, bool success);

protected:
  /*
   * The following functions must be implemented by the subclass to interact
   * agnostically with the portal.
   */

  /*!
   * \brief returns as a string the API this manager represents. This is used to
   * populate the title of sample viewer for the user.
   * \return API as human-readable string.
   */
  virtual QString api() const;

  /*
   * Clears any cached named user credentials so apps that use named user
   * workflows will prompt a challenge each time they are opened.
   */
  virtual void clearCredentialCache() = 0;

  /*!
   * \brief Calls the load function on the internal portal.
   */
  virtual void loadPortal() = 0;

  /*!
   * \brief tests if the portal of the sample manager has been loaded or not.
   * \return true if loaded, false otherwise.
   */
  virtual bool isPortalLoaded() const = 0;

  /*!
   * \brief Creates and stores a portal item as the current portal item.
   * \param itemId to generate the portal item with.
   */
  virtual void createPortalItem(const QString& itemId) = 0;

  /*!
   * \brief fetchData calls fetchData on the portalItem generated by the last
   *        call to `createPortalItem`.
   * \param outputPath fully qualified system path the portal item will be
   *        downloaded to.
   */
  virtual void fetchData(const QString& outputPath) = 0;

protected:
  QString m_outputPath;
  QString m_formattedPath;

private:
  QString formattedPath(const QString& outputPath,
                        const QString& folderName = QString());

};

#endif // CPPSAMPLEMANAGER_H
