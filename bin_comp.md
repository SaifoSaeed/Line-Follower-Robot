To find the values 6 and 9 mathematically, we use a method called binary decomposition (or finding the closest powers of 2).

We are trying to approximate the division 581​ using only addition and bit-shifts.

Here is the step-by-step math:
1. Find the Target Decimal

First, we need to know what fraction we are trying to build.
Target=581​≈0.017241
2. Find the First "Big" Chunk (Shift 6)

We look for the largest power of 2 that fits into our target (0.017241).

    1/32=0.03125 (Too big)

    1/64=0.015625 (Fits!)

This is our first component. Since 64=26, this corresponds to a Right Shift of 6 (>> 6).
3. Find the "Remainder" Chunk

Now we calculate what is missing from our approximation.
Remainder=Target−First Chunk
Remainder=0.017241−0.015625=0.001616
4. Find the Closest Match for the Remainder (Shift 9)

Now we look for a power of 2 that is close to this small remainder (0.001616).

    1/256=0.003906 (Too big)

    1/512≈0.001953 (Close!)

    1/1024≈0.000976 (Too small)

The closest match is 1/512. Since 512=29, this corresponds to a Right Shift of 9 (>> 9).
5. Combine Them

When we add these two parts together:
641​+5121​≈0.017578

Compare this to our original target (581​≈0.017241):

    Target: 0.01724

    Approximation: 0.01757

This is extremely close (only about 1.9% error), which is accurate enough for a simple robot obstacle sensor.

The Code:
// (Value / 64) + (Value / 512)
return (t1_counts >> 6) + (t1_counts >> 9);