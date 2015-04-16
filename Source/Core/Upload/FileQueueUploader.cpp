/*

    Image Uploader -  free application for uploading images/files to the Internet

    Copyright 2007-2015 Sergey Svistunov (zenden2k@gmail.com)

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

#include "FileQueueUploader.h"
#include <algorithm>
#ifndef IU_CLI
#include <zthread/Thread.h>
#include <zthread/Mutex.h>
#endif
#include <Core/Upload/UploadTask.h>
#include <Core/Upload/FileUploadTask.h>


#ifndef IU_CLI
	#include "Gui/Dialogs/LogWindow.h"
#endif
#include "Core/Upload/Uploader.h"

#include "FileQueueUploaderPrivate.h"
/* public CFileQueueUploader class */

CFileQueueUploader::CFileQueueUploader()
{
	_impl = new FileQueueUploaderPrivate(this);
}

void CFileQueueUploader::addSession(std::shared_ptr<UploadSession> uploadSession)
{
}

void CFileQueueUploader::AddFile(const std::string& fileName, const std::string& displayName, void* user_data, CAbstractUploadEngine *uploadEngine)
{
	_impl->AddFile(fileName, displayName, user_data, uploadEngine);
}

bool CFileQueueUploader::start()
{
	_impl->start();
	return true;
}

void CFileQueueUploader::setCallback(Callback* callback)
{
	_impl->callback_ = callback;
}

void CFileQueueUploader::stop()
{
	_impl->m_NeedStop = true;
}

bool CFileQueueUploader::IsRunning() const {
	return _impl->m_IsRunning;
}

void CFileQueueUploader::setUploadSettings(CAbstractUploadEngine* engine)
{
	_impl->m_engine = engine;
}

CFileQueueUploader::~CFileQueueUploader() {
	delete _impl;
}


void CFileQueueUploader::setMaxThreadCount(int threadCount) {
	_impl->m_nThreadCount = threadCount;
}

bool CFileQueueUploader::isSlotAvailableForServer(std::string serverName, int maxThreads) {
	int threads = _impl->serverThreads_[serverName].runningThreads + _impl->serverThreads_[serverName].waitingFileCount;
	return threads < maxThreads && threads < _impl->m_nThreadCount;
}

void CFileQueueUploader::addUploadFilter(UploadFilter* filter)
{
}

void CFileQueueUploader::removeUploadFilter(UploadFilter* filter)
{
}

void CFileQueueUploader::addTask(std_tr::shared_ptr<UploadTask> task) {
	_impl->AddTask(task);
}
