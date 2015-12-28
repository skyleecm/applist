This is a S60 python extension module for getting list of applications info.

The applist function returns a list of tuples (Uid, caption, fullName).
Uid is an integer.

import applist

apps = applist.applist()

This is used in SkyExplorer.