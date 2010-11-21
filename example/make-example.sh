#!/bin/bash

mkdir -p build

cat buttons.html | ../bin/image-nuggets \
                                    --from=html \
                                    --to=png \
                                    --optipng-level=2 \
                                    --dest-prefix=build/ \
                                    landing/lorem.png \
                                    landing/lorem_hover.png \
                                    landing/lorem_active.png

