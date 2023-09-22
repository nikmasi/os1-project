.global _ZN7_thread13contextSwitchEPNS_7ContextES1_
.type _ZN7_thread13contextSwitchEPNS_7ContextES1_, @function
_ZN7_thread13contextSwitchEPNS_7ContextES1_:
    sd ra, 0 * 8(a0) #a0 - &old->context
    sd sp, 1 * 8(a0)

    ld ra, 0 * 8(a1) #a1 - &new->context
    ld sp, 1 * 8(a1)

    ret