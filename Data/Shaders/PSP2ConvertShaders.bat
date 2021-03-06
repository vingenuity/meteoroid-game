@echo off
	echo Converting vertex shaders...
	for /R "." %%f in (*.vertex.cg) do (
		echo 	Converting %%~nf%%~xf...
		psp2cgc.exe --cache --profile sce_vp_psp2 %%f -o "%%~nf.gxp"
)
	echo Converting fragment shaders...
	for /R "." %%f in (*.fragment.cg) do (
		echo 	Converting %%~nf%%~xf...
		psp2cgc.exe --cache --profile sce_fp_psp2 %%f -o "%%~nf.gxp"
)
pause