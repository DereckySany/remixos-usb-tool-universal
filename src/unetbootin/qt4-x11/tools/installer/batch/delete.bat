:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
::
:: Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
:: Contact: Qt Software Information (qt-info@nokia.com)
::
:: This file is part of the Windows installer of the Qt Toolkit.
::
:: $QT_BEGIN_LICENSE:LGPL$
:: Commercial Usage
:: Licensees holding valid Qt Commercial licenses may use this file in
:: accordance with the Qt Commercial License Agreement provided with the
:: Software or, alternatively, in accordance with the terms contained in
:: a written agreement between you and Nokia.
::
:: GNU Lesser General Public License Usage
:: Alternatively, this file may be used under the terms of the GNU Lesser
:: General Public License version 2.1 as published by the Free Software
:: Foundation and appearing in the file LICENSE.LGPL included in the
:: packaging of this file.  Please review the following information to
:: ensure the GNU Lesser General Public License version 2.1 requirements
:: will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
::
:: In addition, as a special exception, Nokia gives you certain
:: additional rights. These rights are described in the Nokia Qt LGPL
:: Exception version 1.0, included in the file LGPL_EXCEPTION.txt in this
:: package.
::
:: GNU General Public License Usage
:: Alternatively, this file may be used under the terms of the GNU
:: General Public License version 3.0 as published by the Free Software
:: Foundation and appearing in the file LICENSE.GPL included in the
:: packaging of this file.  Please review the following information to
:: ensure the GNU General Public License version 3.0 requirements will be
:: met: http://www.gnu.org/copyleft/gpl.html.
::
:: If you are unsure which license is appropriate for your use, please
:: contact the sales department at qt-sales@nokia.com.
:: $QT_END_LICENSE$
::
:: This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
:: WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
::
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
call :%1 %2
goto END

:destDir
  if exist "%IWMAKE_OUTDIR%\%~1" rd /S /Q %IWMAKE_OUTDIR%\%~1
goto :eof

:dir
  if exist "%IWMAKE_ROOT%\%~1" rd /S /Q %IWMAKE_ROOT%\%~1
goto :eof

:dirAbs
  if exist "%~1"  rd /S /Q %~1
goto :eof

:file
  del /Q /F %IWMAKE_OUTDIR%\%~1 >> %IWMAKE_LOGFILE% 2>&1
  exit /b 0
goto :eof

:files
  del /S /Q /F %IWMAKE_OUTDIR%\%~1 >> %IWMAKE_LOGFILE% 2>&1
  exit /b 0
goto :eof

:line
  for /F "tokens=1*" %%m in ("%~1") do set IWMAKE_TMP=%%~m& set IWMAKE_TMP2=%%~n
  if exist "%IWMAKE_ROOT%\tmp_line.txt" del /Q /F "%IWMAKE_ROOT%\tmp_line.txt" >> %IWMAKE_LOGFILE%
  type "%IWMAKE_ROOT%\%IWMAKE_TMP%" | find /V "%IWMAKE_TMP2%" >> "%IWMAKE_ROOT%\tmp_line.txt"
  xcopy /Y /Q /R %IWMAKE_ROOT%\tmp_line.txt %IWMAKE_ROOT%\%IWMAKE_TMP% >> %IWMAKE_LOGFILE%
goto :eof

:END
