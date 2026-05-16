using u128 = __uint128_t;

ll mod_mul(ll a, ll b, ll mod) {
    return (u128)a * b % mod;
}

ll mod_pow(ll a, ll d, ll mod) {
    ll r = 1;
    while (d) {
        if (d & 1) r = mod_mul(r, a, mod);
        a = mod_mul(a, a, mod);
        d >>= 1;
    }
    return r;
}

bool isPrime(ll n) {
    if (n < 2) return false;
    for (ll p : {2, 3, 5, 7, 11, 13, 17, 19, 23, 29}) {
        if (n % p == 0) return n == p;
    }

    ll d = n - 1, s = 0;
    while ((d & 1) == 0) d >>= 1, s++;

    for (ll a : {2, 325, 9375, 28178, 450775, 9780504, 1795265022}) {
        if (a % n == 0) continue;
        ll x = mod_pow(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (ll r = 1; r < s; r++) {
            x = mod_mul(x, x, n);
            if (x == n - 1) {
                composite = false;
                break;
            }
        }
        if (composite) return false;
    }
    return true;
}

mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());

ll pollard(ll n) {
    if (n % 2 == 0) return 2;
    if (n % 3 == 0) return 3;

    ll c = uniform_int_distribution<ll>(1, n - 1)(rng);
    ll x = uniform_int_distribution<ll>(0, n - 1)(rng);
    ll y = x;
    ll d = 1;

    auto f = [&](ll x) {
        return (mod_mul(x, x, n) + c) % n;
    };

    while (d == 1) {
        x = f(x);
        y = f(f(y));
        d = gcd(abs(x - y), n);
    }

    if (d == n) return pollard(n);
    return d;
}

void factor(ll n, map<ll, ll>& res) {
    if (n == 1) return;
    if (isPrime(n)) {
        res[n]++;
        return;
    }
    ll d = pollard(n);
    factor(d, res);
    factor(n / d, res);
}