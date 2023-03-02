$fn=100;

color([0, 49/255, 83/255])laemp_base(4, 4, false);

module laemp_base(h=10, h_lower=5, thread=false){
    union(){
        // upper hexagon
        difference(){
            rotate([0,0,90])hexagon(40,h);
            translate([0,0,-0.5])rotate([0,0,90])hexagon(36,h+1);
        }
        // lower hexagon
        translate([0,0, -h_lower])
        difference(){
            rotate([0,0,90])hexagon(40+1.6,h_lower);
            translate([0,0,-0.5])rotate([0,0,90])hexagon(36,h_lower+1);
        }
        // screwholes
        translate([-17,20,-h_lower])mounting_holes(2.5,h+h_lower,2);
        translate([17,-20,-h_lower])mounting_holes(2.5,h+h_lower,2);
        // bridges
        translate([-17,20,h/2-h_lower/2]){
            rotate([0,0,90])translate([3+2,0,0])cube([6,2,h+h_lower],center=true);
            rotate([0,0,170])translate([5+2,0,0])cube([10,2,h+h_lower],center=true);
        }
        translate([17,-20,h/2-h_lower/2]){
            rotate([0,0,-90])translate([3+2,0,0])cube([6,2,h+h_lower],center=true);
            rotate([0,0,-10])translate([5+2,0,0])cube([10,2,h+h_lower],center=true);
        }
    }
}

module thread(do=10,di=9,s=1.9,h=10,res=5){
    ssize=360/res;
    for(i=[1:res:360/s*h]){
        rotate([0,0,i])translate([(do-di)/2,0,s*i/360-0.01])cylinder(h=s/ssize+0.02,d=di);
    }
}

module mounting_holes(sDia=3,h=1,wall=2){
    difference(){
        cylinder(d=wall*2+sDia,h=h);
        translate([0,0,-0.5])cylinder(d=sDia,h=h+1);
    }
}

module hexagon(x = 10, h = 1){
    cylinder(d=x*2, h=h, $fn=6);
}