#include <pebble.h>
#pragma once

static const GPathInfo TREE_PATH_POINTS = {
  	15,
  	(GPoint []) {
    	{0, -80},
		{48, -32},
		{24, -32},
		{56, 0},
		{32, 0},
		{64, 32},
		{16, 32},
		{16, 64},
		{-16, 64},
		{-16, 32},
		{-64, 32},
		{-32, 0},
		{-56, 0},
		{-24, -32},
		{-48, -32}
	}
};

static const GPathInfo TREE_BOX_POINTS = {
	4,
	(GPoint []) {
		{22, 40},
		{22, 72},
		{-22, 72},
		{-22, 40}
	}
};

static const GPathInfo STAR_PATH_POINTS = {
	10,
	(GPoint []) {
		{0, -10},
		{3, -3},
		{9, -3},
		{4, 2},
		{6, 8},
		{0, 4},
		{-6, 8},
		{-4, 2},
		{-9, -3},
		{-3, -3}
	}
};
