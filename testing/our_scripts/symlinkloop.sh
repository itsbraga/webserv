#!/bin/bash

ln -s b.conf a.conf
ln -s a.conf b.conf
stat -L a.conf 2>&1
rm a.conf b.conf
