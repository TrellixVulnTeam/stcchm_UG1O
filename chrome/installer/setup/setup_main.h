// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// This header exists as a starting point for extracting some of the
// logic out of setup_main.cc.

#ifndef CHROME_INSTALLER_SETUP_SETUP_MAIN_H_
#define CHROME_INSTALLER_SETUP_SETUP_MAIN_H_

#include "chrome/installer/util/util_constants.h"

namespace base {
class CommandLine;
class FilePath;
}  // namespace base

namespace installer {
class InstallationState;
class InstallerState;
class MasterPreferences;

// Helper function that performs the installation of a set of products.
// |installer_directory|, if non-NULL, is populated with the path to the
// directory containing the newly installed setup.exe. |archive_type| is
// populated with the type of archive found. |delegated_to_existing| is set to
// |true| if installation was delegated to a pre-existing higher version.
InstallStatus InstallProductsHelper(const InstallationState& original_state,
                                    const base::FilePath& setup_exe,
                                    const base::CommandLine& cmd_line,
                                    const MasterPreferences& prefs,
                                    InstallerState& installer_state,
                                    base::FilePath* installer_directory,
                                    ArchiveType* archive_type);

}  // namespace installer

#endif  // CHROME_INSTALLER_SETUP_SETUP_MAIN_H_
