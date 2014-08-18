$fn = 50;

// left leg
difference() {
	cube([13, 12, 3]);
	translate([5, 6, 0])
		cylinder(r = 1.6, h = 6);
}

// right leg
translate([30, 0, 0])
difference() {
	cube([13, 12, 3]);
	translate([8, 6, 0])
		cylinder(r = 1.6, h = 6);
}

// left upright
translate([10, 0, 0])
cube([3, 12, 22]);

// right upright
translate([30, 0, 0])
cube([3, 12, 22]);

// crossbar
translate([10, 0, 22])
cube([23, 12, 3]);