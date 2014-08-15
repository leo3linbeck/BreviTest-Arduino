p_y = 1.5; // path width
p_x = 4; // path length
w_x = 6; // well length
w_y = 8; // well width
n = 8; // number of wells 
m = 8; //separation from midline

module base_card()
{translate([-5,-20])square([100,40]);}

module corner(f)
{difference(){square([f,f]);circle(r=f,$fn=50);}}

module path()
{union(){
	translate([w_x/2,-w_y/2])square([p_x,p_y]); // actual path
	translate([w_x/2+1,-w_y/2+p_y+1])rotate(180)corner(1);}}//rounded corners

module path2()
{union(){translate([2*w_x+1,-w_y/2])square([p_x,p_y]); // actual path
			 //translate([2*w_x+2,-p_y/2-1])rotate(90)corner(1);
	      translate([2*w_x+2,-w_y/2+1+p_y])rotate(180)corner(1);//rounded corners	
	      //translate([2*w_x+p_x,-p_y/2-1])corner(1);
	      translate([2*w_x+p_x,-w_y/2+1+p_y])rotate(270)corner(1);}}//rounded corners

module well()
{difference ()
	{	square([w_x,w_y], center=true); 
		translate([w_x/2-2,w_y/2-2])corner(2);
		translate([-(w_x/2-2),w_y/2-2])rotate(90)corner(2);
		//translate([-(w_x/2-2),-(w_y/2-2)])rotate(180)corner(2);
		//translate([w_x/2-2,-(w_y/2-2)])rotate(270)corner(2);
   }}

module color_well() // last well
{translate([n*(w_x+p_x)+1,0])
{difference ()
	{	square([w_y,w_y], center=true); 
		translate([w_y/2-2,w_y/2-2])corner(2);
		translate([-(w_y/2-2),w_y/2-2])rotate(90)corner(2);
		//translate([-(w_y/2-2),-(w_y/2-2)])rotate(180)corner(2);
		//translate([w_y/2-2,-(w_y/2-2)])rotate(270)corner(2);
   }}}

module sample_well() // first well
{union(){translate([0,-w_y/2-1])square([2*w_x+1,w_y+2]); path2();}}

module control_well() // first well
{union(){translate([p_x+w_x,0])well(); path2();}}

module set(){union(){color_well(); 
	{for (i =[2:(n-1)]){translate([i*(p_x+w_x),0]){well();path();}}} }} // 1 set of wells, paths and comb

difference(){base_card(); 
					translate([0,-m]){sample_well(); set(); square([12,6],center=true);} 
					translate([0,m]){control_well(); set();}}







