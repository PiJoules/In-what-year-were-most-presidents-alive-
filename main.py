#!/usr/bin/env python
# -*- coding: utf-8 -*-

from __future__ import print_function

import sys
import re

from collections import defaultdict


YEAR_PAT = re.compile("\d{4}")


def parse_date(line):
    """Get the date from an entry in the csv."""
    match = YEAR_PAT.search(line)
    if match:
        return int(match.group())
    return 9999


class YearRange(object):

    def __init__(self, years=None):
        self.__years = years or defaultdict(int)

    def add_year(self, year):
        self.__years[year] += 1

    @classmethod
    def from_start_end(cls, start, end):
        years = defaultdict(int)
        for i in xrange(start, end + 1):
            years[i] += 1
        return cls(years=years)

    @property
    def years(self):
        return self.__years

    def __add__(self, other):
        years = defaultdict(int)
        for year, count in self.years.iteritems():
            years[year] += count
        for year, count in other.years.iteritems():
            years[year] += count
        return YearRange(years=years)


def main():
    yr = YearRange()
    for line in sys.stdin:
        line = line.strip()
        if not line:
            continue
        line = line.split(",")
        born = parse_date(line[1].strip())
        death = parse_date(line[3].strip())
        yr += YearRange.from_start_end(born, death)
    print(max(yr.years.iteritems(), key=lambda x: x[1]))

    return 0


if __name__ == "__main__":
    sys.exit(main())
