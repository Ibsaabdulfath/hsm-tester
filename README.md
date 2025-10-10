
# HSM Tester

## Overview

I created this project years ago to validate cryptographic operations on a Hardware Security Module (HSM) or any PKCS#11-compliant cryptographic provider. It was originally developed around 2013 for Linux environments as part of an internal FIPS/CAVS verification workflow.

The tool parses **NIST CAVS test vector files** and automatically executes the corresponding cryptographic operations through a **PKCS#11 interface**, using a command-line utility (such as `pkcs11-tool` from the OpenSC project) as the execution backend. It was used to verify correctness and interoperability of HSM implementations against standardized algorithm test sets.
