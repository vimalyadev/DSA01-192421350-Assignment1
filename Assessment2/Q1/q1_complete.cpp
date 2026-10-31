// Q1 – Complete the Code: Hostel Fee Concession Updater
// TODO 1 filled: reduce caller's fee by percent using call-by-reference
#include <iostream>

void applyByValue(long long fee, int percent)
{
    fee -= fee * percent / 100; // changes only the local copy
}

void applyConcession(long long& fee, int percent)
{
    // TODO 1 COMPLETED: Reduce the caller's fee by percent using integer arithmetic
    fee -= fee * percent / 100;
}

int main()
{
    long long hostelFee = 45000;
    applyByValue(hostelFee, 10);
    std::cout << "after call by value : " << hostelFee << '\n';
    applyConcession(hostelFee, 10);
    std::cout << "after call by reference: " << hostelFee << '\n';
}

/*
 * EXPECTED OUTPUT:
 * after call by value : 45000
 * after call by reference: 40500
 *
 * EXPLANATION:
 * applyByValue receives a copy of hostelFee on the stack.
 * Any modification to that copy does NOT affect the original variable
 * in main(), so hostelFee stays 45000 after the call.
 *
 * applyConcession receives a reference (alias) to the original variable.
 * fee -= fee * percent / 100  =>  45000 -= 45000*10/100 = 4500  => 40500.
 * This writes through the reference directly to hostelFee in main().
 */
