#!/usr/bin/python
# -*- coding: utf-8 -*-

import bint as lib
import sys
import random
import argparse


def miller_rabin_pass(a, s, d, n):
    a_to_power = pow(a, d, n)

    if a_to_power == 1:
        return True

    for i in xrange(s - 1):
        if a_to_power == n - 1:
            return True
        a_to_power = (a_to_power * a_to_power) % n

    return a_to_power == n - 1


def miller_rabin(n):
    d = n - 1
    s = 0
    while d % 2 == 0:
        d >>= 1
        s += 1

    for repeat in xrange(20):
        a = 0
        while a == 0:
            a = random.randrange(n)
        if not miller_rabin_pass(a, s, d, n):
            return False

    return True


def prime_test(num):
    if not miller_rabin(num):
        raise ValueError("Выбранное число не является простым.")


def getArgs():
    parser = argparse.ArgumentParser()
    parser.add_argument('inFile')
    parser.add_argument('outFile')
    parser.add_argument('mode', choices=['e', 'd'])
    return parser.parse_args()


def xgcd(a, b):
    if a == lib.bint(0):
        return 0, 1, b

    if b == lib.bint(0):
        return 1, 0, a

    px = lib.bint(0)
    ppx = lib.bint(1)
    py = lib.bint(1)
    ppy = lib.bint(0)

    while b > lib.bint(0):
        q = a / b
        a, b = b, a % b
        x = ppx - q * px
        y = ppy - q * py
        ppx, px = px, x
        ppy, py = py, y

    return ppx, ppy, a


def inverse(a, p):
    x, y, g = xgcd(a, p)
    return (x % p + p) % p


def gen_keys():
    with open("p.txt") as f:
            p = int(f.read()) 
	
    prime_test(p)
    while True:
        g = random.randint(2, p - 1)
        if ((p - 1) % g) != 1:
            break

    x = random.randint(2, p - 1)
    p = lib.bint(str(p))
    g = lib.bint(str(g))
    x = lib.bint(str(x))
    y = p.powmod(g, x, p)
    pub_key = "{}\n{}\n{}".format(p, g, y)
    priv_key = "{}\n{}".format(x, p)
    return pub_key, priv_key,p, g, y, x


def encryption (g, m, p, y):
    m = lib.bint(str(m))
    if m >= p:
        raise ValueError('Message is too large!')

    pp = p.st()
    pp = int(pp)
    k = random.randint(2, pp - 1)
    k = lib.bint(str(k))
    a = p.powmod(g,k,p)
    b = p.powmod(y,k,p)
    b = (b * m ) % p
    return a, b	


def decryption (a, b, p, x):
    decode_msg = p.powmod(a, x, p)                                     
    decode_msg = inverse(decode_msg, p)
    decode_msg = (decode_msg * b) % p
    return decode_msg


def main():
    print "Elgamal"
    args = getArgs()
    with open(args.inFile) as f:
            msg = int(f.read()) 
    if args.mode == 'e':
        pub_key, priv_key, p, g, y, x = gen_keys()
        a, b = encryption(g, msg, p, y)
        ab = "{}\n{}".format(a, b)
        with open(args.outFile, 'w') as ff:
            ff.write(ab)
        with open('pub.key', 'w') as pub:
            pub.write(pub_key)
        with open('priv.key', 'w') as priv:
            priv.write(priv_key)

	
    if args.mode == "d":
        with open(args.outFile) as f:
            (a, b) = f.read().split("\n")
        a = lib.bint(a)
        b = lib.bint(b) 
        with open('priv.key') as priv:
            (x, p) = priv.read().split("\n")
        x = lib.bint(x)
        p = lib.bint(p)
        decode_msg = decryption(a, b, p, x)
        with open('decode_msg.txt', 'w') as ff:
            ff.write(decode_msg.st())
	

if __name__ == "__main__":
    main()
    
