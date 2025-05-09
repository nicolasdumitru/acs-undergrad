## Diferențe și asemănări între algoritmii Prim și Kruskal

### Asemănări
- Ambii construiesc un **arbore parțial de cost minim (MST)** într-un graf neorientat presupus conex.
- Ambii folosesc o strategie **greedy**.
- Complexitatea lor este influențată de numărul de noduri și muchii.
- Incomod de implementat în cod citibil, de calitate industrială, fără OOP.

---

### 🔀 Diferențe

| Caracteristică       | **Prim**                                                     | **Kruskal**                                                 |
|-------------------|-----------------------------------------------------------------|-------------------------------------------------------------|
| Tip strategie     | Greedy, creștere incrementală                                   | Greedy, sortare și unire de componente                      |
| Punct de pornire  | Pornește dintr-un **nod** ales                                  | Pornește cu toate nodurile izolate                          |
| Ordine operații   | Adaugă cea mai ieftină muchie ce extinde arborele curent        | Sortează toate muchiile după cost și le adaugă una cîte una |
| Evitare cicluri   | Evită cicluri alegînd doar muchii care nu formează ciclu        | Folosește **Union-Find** pentru a detecta cicluri           |
| Structuri de date | Coadă de priorități (heap - care mi-aș fi dorit să fie din STL) | Structură de tip **disjoint set forest (Union-Find)**       |
| Eficiență         | Mai eficient pe grafuri **dense**                               | Mai eficient pe grafuri **sparse** (rare)                   |

---
