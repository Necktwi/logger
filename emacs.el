;;; Package --- Summary
;;; Commentary:
;;; It sets cmake-ide variables

;;; Code:
(defvar CMakeProject "logger")
(require 'cmake-ide)

(setq cmake-ide-build-dir
      (concat cmake-ide-project-dir "build/Linux/x86-64/debug/"))
(setq cmake-ide-build-pool-dir cmake-ide-build-dir)
(make-directory cmake-ide-build-dir t)
(setq cmake-ide-cmake-opts
      (concat "-D_DEBUG=1 -DCMAKE_BUILD_TYPE=Debug "
	      "-DCMAKE_EXPORT_COMPILE_COMMANDS=1"))
(defvar MakeThreadCount 1)
(defvar DebugTarget "logger")
(defvar TargetArguments "-s normal")
(message "logger emacs project loaded.")
(provide 'logger)
;;; emacs.el ends here
