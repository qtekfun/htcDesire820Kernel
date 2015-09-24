// Copyright (c) 2008 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#include <unistd.h>
#include <stdio.h>
#include <leptonica/allheaders.h>

static int
usage(const char *argv0) {
  fprintf(stderr, "Usage: %s [--highlight] [--no-ignore-scrollbars] "
                  "[--output filename] "
                  "<input a> <input b>\n", argv0);
  return 1;
}

int
main(int argc, char **argv) {
  if (argc < 3)
    return usage(argv[0]);

  char highlight = 0;
  char ignore_scrollbars = 1;
  
  const char *output_filename = "highlight.png";

  int argi = 1;

  for (; argi < argc; ++argi) {
    if (strcmp("--highlight", argv[argi]) == 0) {
      highlight = 1;
    } else if (strcmp("--no-ignore-scrollbars", argv[argi]) == 0) {
      ignore_scrollbars = 0;
    } else if (strcmp("--output", argv[argi]) == 0) {
      if (argi + 1 >= argc) {
        fprintf(stderr, "missing argument to --output\n");
        return 1;
      }
      output_filename = argv[++argi];
    } else {
      break;
    }
  }

  if (argc - argi < 2)
    return usage(argv[0]);

  PIX *a = pixRead(argv[argi]);
  PIX *b = pixRead(argv[argi + 1]);

  if (!a) {
    fprintf(stderr, "Failed to open %s\n", argv[argi]);
    return 1;
  }

  if (!b) {
    fprintf(stderr, "Failed to open %s\n", argv[argi + 1]);
    return 1;
  }

  if (pixGetWidth(a) != pixGetWidth(b) ||
      pixGetHeight(a) != pixGetHeight(b)) {
    fprintf(stderr, "Inputs are difference sizes\n");
    return 1;
  }

  PIX *delta = pixAbsDifference(a, b);
  pixInvert(delta, delta);
  if (!highlight)
    pixDestroy(&a);
  pixDestroy(&b);

  PIX *deltagray = pixConvertRGBToGray(delta, 0, 0, 0);
  pixDestroy(&delta);

  PIX *deltabinary = pixThresholdToBinary(deltagray, 254);
  PIX *deltabinaryclipped;
  const int clipwidth = pixGetWidth(deltabinary) - 15;
  const int clipheight = pixGetHeight(deltabinary) - 15;

  if (ignore_scrollbars && clipwidth > 0 && clipheight > 0) {
    BOX *clip = boxCreate(0, 0, clipwidth, clipheight);

    deltabinaryclipped = pixClipRectangle(deltabinary, clip, NULL);
    boxDestroy(&clip);
    pixDestroy(&deltabinary);
  } else {
    deltabinaryclipped = deltabinary;
    deltabinary = NULL;
  }

  PIX *hopened = pixOpenBrick(NULL, deltabinaryclipped, 3, 1);
  PIX *vopened = pixOpenBrick(NULL, deltabinaryclipped, 1, 3);
  pixDestroy(&deltabinaryclipped);

  PIX *opened = pixOr(NULL, hopened, vopened);
  pixDestroy(&hopened);
  pixDestroy(&vopened);

  l_int32 count;
  pixCountPixels(opened, &count, NULL);
  fprintf(stderr, "%d\n", count);

  if (count && highlight) {
    PIX *d1 = pixDilateBrick(NULL, opened, 7, 7);
    PIX *d2 = pixDilateBrick(NULL, opened, 3, 3);
    pixInvert(d2, d2);
    pixAnd(d1, d1, d2);
    pixPaintThroughMask(a, d1, 0, 0, 0xff << 24);
    pixWrite(output_filename, a, IFF_PNG);
  }

  return count > 0;
}
