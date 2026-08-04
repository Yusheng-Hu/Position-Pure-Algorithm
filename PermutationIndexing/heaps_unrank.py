# Unrank / rank of Heap permutations
# Lehmer code C, most-significant digit first:
#   C[0]×(n-1)! + ... + C[n-2]×1! + C[n-1]×0!
#
# Fractal self-similarity: the block structure of Heap(k) is the accumulated
# result of the rearrangement rules of lower levels, layer by layer.
# Rearrangement rule (formerly _step, now inlined): even = swap ends,
# odd = cyclic shift.
#   Equivalent position-map enumerations: k=1→[0]  k=2→[1,0]  k=3→[2,1,0]
#                                        k=4→[1,2,3,0]  k=5→[4,1,2,3,0]  k=6→[3,4,1,2,5,0]
# The trailing-value sequence mn is not independent — it is exactly the old
# value overwritten by the swap-in action while S advances.
#
# Examples (C in user's convention: 0-based, smallest digit first; D = permutation):
#   C = [0,1,1,1,2] -> unrank(5, C[::-1]) == [4,1,3,0,2]   (D_4_1_3_0_2)
#   C = [0,0,0,2,1] -> unrank(5, C[::-1]) == [4,3,0,2,1]   (D_4_3_0_2_1)
#   C = [0,1,0,2,1] -> unrank(5, C[::-1]) == [3,4,0,2,1]   (D_3_4_0_2_1)
#   C = [0,0,1,2,1] -> unrank(5, C[::-1]) == [0,4,3,2,1]   (D_0_4_3_2_1)
# and conversely rank(5, D) == C[::-1] (code-internal C is most-significant first).

# --- Core algorithms ---
def unrank(n, C):
    """Lehmer code C → Heap permutation perm (bottom-up snowball)"""
    # k=1: seed [0]; k=2: only 2 permutations, b=0 → [0,1], b=1 → [1,0]
    if n == 1:
        return [0]
    perm = [C[n - 2], 1 - C[n - 2]]
    if n == 2:
        return perm
    # k=3: swap ends + swap-in at position 0
    S, last = [0, 1], 2
    for _ in range(C[n - 3]):
        S[0], S[1] = S[1], S[0]
        last, S[0] = S[0], last
    perm = [S[x] for x in perm] + [last]

    # Main loop k=4 → n (even: swap ends, odd: cyclic shift); perm has length k-1
    for k in range(4, n + 1):
        S, last = list(range(k - 1)), k - 1  # S: position→value map; last: trailing value
        for j in range(C[n - k]):            # advance to block C[n-k] (C is most-significant first)
            if k % 2 == 0:                   # even: swap ends, swap-in at position j
                S[0], S[k-2] = S[k-2], S[0]
                last, S[j] = S[j], last
            else:                            # odd: cyclic shift, swap-in at position 0
                S = [S[k-4], S[k-3]] + S[1:k-4] + [S[k-2], S[0]]
                last, S[0] = S[0], last
        perm = [S[x] for x in perm] + [last] # translate via S, append the trailing value
    return perm

def rank(n, perm):
    """Heap permutation perm → Lehmer code C (top-down peel-off, inverse of unrank)"""
    C = [0] * n
    p = list(perm)

    # Main loop k = n → 4 (even: swap ends, odd: cyclic shift)
    for k in range(n, 3, -1):
        S, last = list(range(k - 1)), k - 1
        for b in range(k):                   # advance until trailing value matches; b = block index
            if p[-1] == last:
                break
            if k % 2 == 0:                   # even: swap ends, swap-in at position b
                S[0], S[k-2] = S[k-2], S[0]
                last, S[b] = S[b], last
            else:                            # odd: cyclic shift, swap-in at position 0
                S = [S[k-4], S[k-3]] + S[1:k-4] + [S[k-2], S[0]]
                last, S[0] = S[0], last
        C[n - k] = b
        p = [S.index(x) for x in p[:-1]]     # drop trailing value, translate back via S⁻¹

    # k=3: swap ends + swap-in at position 0
    if n >= 3:
        S, last = [0, 1], 2
        for b in range(3):                   # advance until trailing value matches; b = block index
            if p[-1] == last:
                break
            S[0], S[1] = S[1], S[0]
            last, S[0] = S[0], last
        C[n - 3] = b
        p = [S.index(x) for x in p[:-1]]

    # k=2: trailing 1 → b=0, trailing 0 → b=1
    if n >= 2:
        C[n - 2] = 1 - p[1]

    return C

# --- Self-test ---
if __name__ == "__main__":
    def heaps_seq(n):   # standard iterative Heap algorithm (ground truth)
        A = list(range(n))
        seq = [A[:]]
        c = [0] * n
        i = 0
        while i < n:
            if c[i] < i:
                if i % 2 == 0: A[0], A[i] = A[i], A[0]
                else: A[c[i]], A[i] = A[i], A[c[i]]
                seq.append(A[:])
                c[i] += 1; i = 0
            else:
                c[i] = 0; i += 1
        return seq

    def next_C(C, n):   # increment Lehmer code
        C[n-2] += 1
        i = n - 2
        while i > 0 and C[i] > n - i - 1:
            C[i] = 0; i -= 1; C[i] += 1

    for n in range(1, 9):
        seq = heaps_seq(n)
        C = [0] * n
        ok = True
        for expect in seq:
            if unrank(n, C) != expect:
                ok = False; break
            next_C(C, n)
        C = [0] * n
        ok2 = True
        for _ in seq:
            if rank(n, unrank(n, C)) != C:
                ok2 = False; break
            next_C(C, n)
        print(f"  [{'PASS' if ok else 'FAIL'}] n={n} unrank"
              f"  [{'PASS' if ok2 else 'FAIL'}] n={n} roundtrip")

    print("\nO(n³) time, O(n) space")
