package main

import (
	"fmt"
	"sort"
	"strings"
)

type Edge struct{ u, v int }
type Rule struct {
	target int
	pre    []int
}

func norm(u, v int) Edge {
	if u > v {
		u, v = v, u
	}
	return Edge{u, v}
}
func nextPerm(a []int) bool {
	i := len(a) - 2
	for i >= 0 && a[i] >= a[i+1] {
		i--
	}
	if i < 0 {
		return false
	}
	j := len(a) - 1
	for a[j] <= a[i] {
		j--
	}
	a[i], a[j] = a[j], a[i]
	for l, r := i+1, len(a)-1; l < r; l, r = l+1, r-1 {
		a[l], a[r] = a[r], a[l]
	}
	return true
}
func keyInts(a []int) string {
	var b strings.Builder
	for _, x := range a {
		b.WriteByte(byte(x + 1))
	}
	return b.String()
}

func main() {
	const n = 9
	edges := make([]Edge, 0, n*(n-1)/2)
	eid := make(map[Edge]int)
	for i := 0; i < n; i++ {
		for j := i + 1; j < n; j++ {
			e := Edge{i, j}
			eid[e] = len(edges)
			edges = append(edges, e)
		}
	}
	qv := []Edge{{0, 1}, {0, 2}, {0, 4}, {1, 3}, {1, 5}, {2, 3}, {2, 6}, {3, 7}, {4, 5}, {4, 6}, {5, 7}, {6, 7}}
	qset := make(map[Edge]bool)
	for _, e := range qv {
		qset[e] = true
	}
	outsideEdgeIDs := make([]int, 0)
	outsideIndex := make(map[int]int)
	for id, e := range edges {
		if !qset[e] {
			outsideIndex[id] = len(outsideEdgeIDs)
			outsideEdgeIDs = append(outsideEdgeIDs, id)
		}
	}
	U := len(outsideEdgeIDs)

	localID := make(map[Edge]int)
	lid := 0
	for i := 0; i < 8; i++ {
		for j := i + 1; j < 8; j++ {
			localID[Edge{i, j}] = lid
			lid++
		}
	}
	base := make(map[string][]int)
	p := []int{0, 1, 2, 3, 4, 5, 6, 7}
	for {
		arr := make([]int, 0, 12)
		for _, e := range qv {
			a, b := p[e.u], p[e.v]
			arr = append(arr, localID[norm(a, b)])
		}
		sort.Ints(arr)
		base[keyInts(arr)] = arr
		if !nextPerm(p) {
			break
		}
	}
	if len(base) != 840 {
		panic(fmt.Sprintf("base cube count %d", len(base)))
	}

	invLocal := make([]Edge, 28)
	for e, i := range localID {
		invLocal[i] = e
	}
	cubes := make(map[string][]int)
	masks := make(map[string][]int)
	choose := func() {}
	_ = choose
	var rec func(start, need int, cur []int)
	rec = func(start, need int, cur []int) {
		if need == 0 {
			V := append([]int(nil), cur...)
			for _, arr := range base {
				ge := make([]int, 0, 12)
				om := make([]int, 0, 12)
				for _, li := range arr {
					le := invLocal[li]
					e := Edge{V[le.u], V[le.v]}
					gid := eid[norm(e.u, e.v)]
					ge = append(ge, gid)
					if oi, ok := outsideIndex[gid]; ok {
						om = append(om, oi)
					}
				}
				sort.Ints(ge)
				sort.Ints(om)
				cubes[keyInts(ge)] = ge
				masks[keyInts(om)] = om
			}
			return
		}
		for x := start; x <= n-need; x++ {
			rec(x+1, need-1, append(cur, x))
		}
	}
	rec(0, 8, nil)
	if len(cubes) != 7560 {
		panic(fmt.Sprintf("cube count %d", len(cubes)))
	}

	rules := make([]Rule, 0)
	for _, om := range masks {
		for _, t := range om {
			pre := make([]int, 0, len(om)-1)
			for _, x := range om {
				if x != t {
					pre = append(pre, x)
				}
			}
			rules = append(rules, Rule{target: t, pre: pre})
		}
	}
	rev := make([][]int, U)
	zeroTargets := make([]int, 0)
	for r, rule := range rules {
		if len(rule.pre) == 0 {
			zeroTargets = append(zeroTargets, rule.target)
		}
		for _, x := range rule.pre {
			rev[x] = append(rev[x], r)
		}
	}

	stateStamp := make([]int, U)
	ruleStamp := make([]int, len(rules))
	have := make([]int, len(rules))
	run := 0
	closure := func(seed []int) int {
		run++
		queue := make([]int, 0, U)
		for _, e := range seed {
			if stateStamp[e] != run {
				stateStamp[e] = run
				queue = append(queue, e)
			}
		}
		for _, e := range zeroTargets {
			if stateStamp[e] != run {
				stateStamp[e] = run
				queue = append(queue, e)
			}
		}
		count := len(queue)
		for head := 0; head < len(queue); head++ {
			e := queue[head]
			for _, r := range rev[e] {
				if ruleStamp[r] != run {
					ruleStamp[r] = run
					have[r] = 0
				}
				have[r]++
				if have[r] == len(rules[r].pre) {
					t := rules[r].target
					if stateStamp[t] != run {
						stateStamp[t] = run
						queue = append(queue, t)
						count++
					}
				}
			}
		}
		return count
	}

	scan := func(k int) (int, int) {
		chosen := make([]int, k)
		per := 0
		maxC := 0
		var walk func(pos, next int)
		walk = func(pos, next int) {
			if pos == k {
				c := closure(chosen)
				if c == U {
					per++
				}
				if c > maxC {
					maxC = c
				}
				return
			}
			for x := next; x <= U-(k-pos); x++ {
				chosen[pos] = x
				walk(pos+1, x+1)
			}
		}
		walk(0, 0)
		return per, maxC
	}
	c4, m4 := scan(4)
	fmt.Printf("distinct_Q3_copies=%d\n", len(cubes))
	fmt.Printf("distinct_outside_edge_masks=%d\n", len(masks))
	fmt.Printf("four_edge_seeds=10626 percolating=%d max_closure=%d\n", c4, m4)
	if c4 != 0 {
		panic("independent Go lower-bound check failed")
	}
	fmt.Println("PASS: independent Go implementation excludes all four-edge seeds")
}
