#!/bin/bash
for p in ESP32-S3-MultiFunction-?.pdf; do
  echo -n "$p..."
  gs -q  -dCompatibilityLevel=1.4 -dSubsetFonts=false -dPDFSETTINGS=/printer -dNOPAUSE -dBATCH -sDEVICE=pdfwrite -sOutputFile=X$p $p -c '.setpdfwrite'
  mv $p $p.bak
  mv X$p $p
  echo "done"
done

