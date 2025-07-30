#include <stdio.h>
#include <pthreads.h>

int main(int argc char **argv)
{
  const t_agora   rules;
  t_philos        *philos;
  
  if (argc != 3)
    return (1);
  rules = parse_args(argv);
  philos = make_philos(rules.n_philos);
  run_table(rules, philos);
  
}