#!/bin/bash

commento=$1;
echo "commento inserito: $commento";
git commit -am "$commento";
git push -u origin master;
