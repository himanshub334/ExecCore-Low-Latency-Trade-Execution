# Failure modes

## Partial network write
`sendmsg()` returns the number of bytes accepted by the kernel. A production wire protocol must frame messages and retain the unsent suffix. The demo treats any short write as a failed transmission and requeues the original order sequence, making retry identity explicit.

## Duplicate fills
Fill sequence IDs are inserted into a Bloom filter before the position is updated. A Bloom filter can have false positives, so production systems should pair it with an exact bounded sequence store when false-positive suppression is unacceptable.

## Concurrent risk updates
Java position updates are protected by a deterministic lock stripe derived from the instrument. This serializes updates for one symbol while permitting unrelated symbols to progress independently.
