#!/usr/bin/awk -f
# Script to compute the CutTubs parameters from a text file with points.
# the file is scanned for definitions like `x = ...` (mind the spaces).
# interpreted are:
#   def = ... // XML tag to output
#   r = ... // radius of the ring, used for position computations
#   phi = ... // segment boundary at this phi
#   z = ... // always pairs of two, -z and +z corner at the last phi
# a segment is output for every consecutive pair of phis with pairs of z's

# define some variables to "undefined"
BEGIN {
    P2_z_l = "";
    P2_z_t = "";
    phi2 = "";
    ndefs = 0;
    l_or_t = "l";
    segment = 0;
    min_z = 1e9;
    max_z = -1e9;
}

# variables to be set in the beginning
match($0, /def = (.*)$/, ary) {
    ndefs++;
    defs[ndefs] = ary[1];
}

match($0, /place = (.*)$/, ary) {
    places[ndefs] = ary[1];
}

match($0, /r = ([0-9.]+)/, ary) {
    r = ary[1];
}

# new phi, move old P2 to P1 to be ready to read the new P2
match($0, /phi = ([0-9.]+)/, ary) {
    phi1 = phi2;
    phi2 = ary[1];
    P1_z_l = P2_z_l;
    P1_z_t = P2_z_t;
    
    P1_x_t = cos(phi1) * r;
    P1_x_l = cos(phi1) * r;
    P1_y_t = sin(phi1) * r;
    P1_y_l = sin(phi1) * r;
    
    P2_x_t = cos(phi2) * r;
    P2_x_l = cos(phi2) * r;
    P2_y_t = sin(phi2) * r;
    P2_y_l = sin(phi2) * r;
}

# z values, frst -z (l side), then +z (t side). 
# The values here always define P2, P1 was saved above.
match($0, /z = ([-0-9.]+)/, ary) {
    z = ary[1];
    # track the min/max to properly place&align later
    if (z < min_z) min_z = z;
    if (z > max_z) max_z = z;
    if (l_or_t  == "l") {
        P2_z_l = z;
        l_or_t = "t";
    } else {
        P2_z_t = z;
        
        # once we have a l/t pair and both sides of the segment defined, 
        # we compute and output the segment
        if (P1_z_l != "" && P1_z_t != "") {
            # each cutting plane is defined by P1-3. P1-2 are corners of the 
            # segment, P3 is at r=0 with the "average" z to get a nice cut.
            P3_z_l = (P1_z_l + P2_z_l) / 2;
            P3_z_t = (P1_z_t + P2_z_t) / 2;
            
            # we only have one dz to position both planes. The anchor is implicitly
            # between the P3's, we have to use an offset later to make the segments 
            # line up correctly.
            dz = (P3_z_t - P3_z_l) / 2;
            offset = (P3_z_t + P3_z_l) / 2;
            
            # the plane is defined by P1-P3 and P2-P3; since P3 is at r=0 we
            # only need the z.
            D1_z_l = P1_z_l - P3_z_l;
            D2_z_l = P2_z_l - P3_z_l;
            
            # the normal is then the cross product...
            n_x_l = (P1_y_l * D2_z_l) - (D1_z_l * P2_y_l);
            n_y_l = (D1_z_l * P2_x_l) - (P1_x_l * D2_z_l);
            n_z_l = (P1_x_l * P2_y_l) - (P1_y_l * P2_x_l);
            
            # ... normalized.
            # flip the sign here (but not for t) since root wants it like that.
            norm = -sqrt(n_x_l^2 + n_y_l^2 + n_z_l^2);
            n_x_l /= norm;
            n_y_l /= norm;
            n_z_l /= norm;
            
            # same game for the t side.
            D1_z_t = P1_z_t - P3_z_t;
            D2_z_t = P2_z_t - P3_z_t;
            
            n_x_t = (P1_y_t * D2_z_t) - (D1_z_t * P2_y_t);
            n_y_t = (D1_z_t * P2_x_t) - (P1_x_t * D2_z_t);
            n_z_t = (P1_x_t * P2_y_t) - (P1_y_t * P2_x_t);
            
            norm = sqrt(n_x_t^2 + n_y_t^2 + n_z_t^2);
            n_x_t /= norm;
            n_y_t /= norm;
            n_z_t /= norm;
            
            # segment counter for the xml snippet
            segment++;

            # save the offset for later
            offsets[segment] = offset;
            
            # output one tag for each def (for skin/core)
            for (def in defs) {
                print("<!-- offset=" offset " -->");
                printf(defs[def], segment);
                print("");
                print(" dz=\"" dz "*mm\" ");
                print(" startPhi=\"" phi1 "*rad\" ");
                print(" deltaPhi=\"" phi2 "*rad\" "); # WTF? Who came up with this?
                print(" lx=\"" n_x_l "\" ly=\"" n_y_l "\" lz=\"" n_z_l "\" ");
                print(" tx=\"" n_x_t "\" ty=\"" n_y_t "\" tz=\"" n_z_t "\" ");
                print("/>");
            }
            
        }
        
        # reset the "side" state machine.
        l_or_t = "l";
    }
}

# print the PosPart tags. The tag itself is passed in, we only compute the 
# offset and add the numbers.
END {
    print("");
    print("<!-- width: " (max_z - min_z) " -->");
    shift = min_z + (max_z-min_z)/2;
    for (seg in offsets) {
        # for the offset, we subtract the min, to get something aligned
        offset = offsets[seg] - shift;
        for (place in places) {
            printf(places[place], seg, offset);
            print("");
        }
    }
}
