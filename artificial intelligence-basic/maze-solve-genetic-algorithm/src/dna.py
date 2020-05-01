import random


# DNA is a array of direction
class DNA:
    def __init__(self, item):                                                  # Constructor for DNA
        self.genes = []                                                        # The genetic sequence
        self.gen_index = 0

        if type(item) == int:                                                  # 1- Generate new genes randomly, or
            rand_gen = 0
            for i in range(0, item):
                rand_gen = self.random_gen(rand_gen)
                self.genes.append(rand_gen)
        else:                                                                  # 2- Take genes from given genes
            self.genes = item

    # Create new DNA sequence from self and partner
    def crossover(self, partner, _random=True):
        child_genes = []

        if _random:                                                            # Random crossover
            midpoint = random.randint(0, len(self.genes) - 1)                         # Select a middle point
            for i in range(0, midpoint):                                       # Take genes from self until midpoint
                child_genes.append(self.genes[i])
            for i in range(midpoint, len(self.genes)):                         # Take the rest genes from partner
                child_genes.append(partner.genes[i])
        else:
            if self.gen_index > partner.gen_index:                             # Until dead crossover
                midpoint = int(self.gen_index * 0.9)
                for i in range(0, midpoint):                                   # Take genes from self until midpoint
                    child_genes.append(self.genes[i])
                for i in range(midpoint, len(self.genes)):                     # Take the rest genes from partner
                    child_genes.append(partner.genes[i])
            else:
                midpoint = int(partner.gen_index * 0.9)
                for i in range(0, midpoint):                                   # Take genes from partner until midpoint
                    child_genes.append(partner.genes[i])
                for i in range(midpoint, len(self.genes)):                     # Take the rest genes from self
                    child_genes.append(self.genes[i])

        child_dna = DNA(child_genes)
        return child_dna
    
    # Based on mutation rate, pick a random direction
    def mutate(self, mutation_rate):
        if random.random() < mutation_rate:
            for i in range(0, len(self.genes)):                                    # For every gen from genes
                if random.random() < mutation_rate:                                # Generate new gen randomly,
                    if i > 0:
                        pre_gen = self.genes[i - 1]
                    else:
                        pre_gen = 0
                    self.genes[i] = self.random_gen(pre_gen)                       # based on mutation probability

    # Get gen from genes[i] and return as a direction
    def get_next(self):
        (x, y) = (0, 0)                                                        # Calculate direction based genes,
        if self.gen_index < len(self.genes):                                   # until genes is finish
            if self.genes[self.gen_index] == 1:
                (x, y) = (x, y - 1)
            elif self.genes[self.gen_index] == 2:
                (x, y) = (x - 1, y)
            elif self.genes[self.gen_index] == 3:
                (x, y) = (x, y + 1)
            else:
                (x, y) = (x + 1, y)
            self.gen_index += 1
        return x, y

    def random_gen(self, previous_gen):
        if previous_gen == 1:
            _list = [1, 2, 4]
        elif previous_gen == 2:
            _list = [1, 2, 3]
        elif previous_gen == 3:
            _list = [2, 3, 4]
        elif previous_gen == 4:
            _list = [1, 3, 4]
        else:
            _list = [1, 2, 3, 4]
        return random.choice(_list)
