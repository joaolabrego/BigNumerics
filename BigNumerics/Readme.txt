
I haven't reached the end yet, but the idea is already launched and quite advanced.

I intend to create a BigNumerics library, which will contain BCD (Binary-Coded Decimal) types, where numbers are represented in vectors with values ​​from 0 to 999,999,999, that is, the numerical base is 10,000,000.
In this format, we have logarithmic, trigonometric, exponential, arithmetic operations, etc. with Integer and Decimal numbers.

Then, I will develop the same operations for the binary format, based on 2 ^ 32, differentiating them by the Big prefix: BigInteger, BigDecimal, BigFloat, etc.

The BigFloat format, contrary to what one might think, is different from BigDecimal.

BigFloat follows the rules defined in IEEE 754, somewhat extended by me, to support numbers of arbitrary precision.

So BigFloat would be a faster format than BigDecimal.

This you will notice when the library is complete.

At the moment, the BCD is almost complete, we just need to implement the Linear Regression and Matrices module.

Anyway, I hope that this work will serve to open your minds about the secrets involved in making a pocket calculator or even a spreadsheet.