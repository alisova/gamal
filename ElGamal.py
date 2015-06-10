#!/usr/bin/python
# -*- coding: utf-8 -*-

import bint as lib
import sys
import random


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
	"""Тест Миллера-Рабина на простоту числа

	"""
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


def xgcd(a, b):
	"""Расширенный алгоритм Евклида

	"""
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
	"""Генерирует ключи

	"""
	f = open("p.txt")

	p = int(f.read())

	f.close()

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

	return p, g, y, x


def elgamal(msg, p, g, y, x):

	msg = lib.bint(str(msg))

	if msg > p:
		raise ValueError("Неверная длина сообщения")

	pp = p.st()

	pp = int(pp)

	k = random.randint(2, pp - 1)

	k = lib.bint(str(k))

	a = p.powmod(g, k, p)                                               # Кодирование

	b = p.powmod(y, k, p)                                               # Кодирование
	b = (msg * b) % p

	decode_msg = p.powmod(a, x, p)                                      # Декодирование
	decode_msg = inverse(decode_msg, p)
	decode_msg = (decode_msg * b) % p

	return decode_msg


def usage():
	print "\nИспользование: python ElGamal.py msg.txt\n"

	sys.exit(-1)


if __name__ == "__main__":
	if len(sys.argv) != 2:
		usage()

	f = open(sys.argv[1])

	msg = int(f.read())

	f.close()

	p, g, y, x = gen_keys()

	decode_msg = elgamal(msg, p, g, y, x)

	f = open("decode_msg.txt", "w")

	f.write(decode_msg.st())

	f.close()
