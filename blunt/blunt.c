#include "blunt.h"

void hotop_setup(void);
void revop_setup(void);

/* --------------------------------------------------------------- */
/*                           connectives                           */
/* --------------------------------------------------------------- */

typedef struct {
	t_blunt bl;
	t_float f;
} t_num;

static void num_float(t_num *x ,t_float f) {
	x->f = f;
	pd_bang((t_pd *)x);
}

static void *num_new(t_class *cl ,t_symbol *s ,int ac ,t_atom *av) {
	t_num *x = (t_num *)pd_new(cl);
	blunt_init(&x->bl ,&x->f ,ac ,av);
	floatinlet_new(&x->bl.obj ,&x->f);
	return (x);
}


/* --------------------- int ------------------------------------- */
static t_class *i_class;

static void i_bang(t_num *x) {
	outlet_float(x->bl.obj.ob_outlet ,(t_float)(int64_t)x->f);
}

static void i_send(t_num *x ,t_symbol *s) {
	if (s->s_thing)
		pd_float(s->s_thing ,(t_float)(int64_t)x->f);
	else pd_error(x ,"%s: no such object" ,s->s_name);
}

static void *i_new(t_symbol *s ,int ac ,t_atom *av) {
	return (num_new(i_class ,s ,ac ,av));
}

/* --------------------- float ----------------------------------- */
static t_class *f_class;

static void f_bang(t_num *x) {
	outlet_float(x->bl.obj.ob_outlet ,x->f);
}

static void f_symbol(t_num *x ,t_symbol *s) {
	t_float f = 0.0f;
	char *str_end = NULL;
	f = strtof(s->s_name ,&str_end);
	if (f == 0 && s->s_name == str_end)
		pd_error(x ,"Couldn't convert %s to float." ,s->s_name);
	else outlet_float(x->bl.obj.ob_outlet ,x->f = f);
}

static void f_send(t_num *x ,t_symbol *s) {
	if (s->s_thing)
		pd_float(s->s_thing ,x->f);
	else pd_error(x ,"%s: no such object" ,s->s_name);
}

static void *f_new(t_symbol *s ,int ac ,t_atom *av) {
	return (num_new(f_class ,s ,ac ,av));
}


/* --------------------------------------------------------------- */
/*                           arithmetics                           */
/* --------------------------------------------------------------- */

/* --------------------- binop1:  + ,- ,* ,/ --------------------- */

/* --------------------- addition -------------------------------- */
static t_class *b1_plus_class;

static void b1_plus_bang(t_bop *x) {
	outlet_float(x->bl.obj.ob_outlet ,blunt_plus(x->f1 ,x->f2));
}

static void *b1_plus_new(t_symbol *s ,int ac ,t_atom *av) {
	return (bop_new(b1_plus_class ,s ,ac ,av));
}

/* --------------------- subtraction ----------------------------- */
static t_class *b1_minus_class;

static void b1_minus_bang(t_bop *x) {
	outlet_float(x->bl.obj.ob_outlet ,blunt_minus(x->f1 ,x->f2));
}

static void *b1_minus_new(t_symbol *s ,int ac ,t_atom *av) {
	return (bop_new(b1_minus_class ,s ,ac ,av));
}

/* --------------------- multiplication -------------------------- */
static t_class *b1_times_class;

static void b1_times_bang(t_bop *x) {
	outlet_float(x->bl.obj.ob_outlet ,blunt_times(x->f1 ,x->f2));
}

static void *b1_times_new(t_symbol *s ,int ac ,t_atom *av) {
	return (bop_new(b1_times_class ,s ,ac ,av));
}

/* --------------------- division -------------------------------- */
static t_class *b1_div_class;

static void b1_div_bang(t_bop *x) {
	outlet_float(x->bl.obj.ob_outlet ,blunt_div(x->f1 ,x->f2));
}

static void *b1_div_new(t_symbol *s ,int ac ,t_atom *av) {
	return (bop_new(b1_div_class ,s ,ac ,av));
}

/* --------------------- log ------------------------------------- */
static t_class *b1_log_class;

static void b1_log_bang(t_bop *x) {
	outlet_float(x->bl.obj.ob_outlet ,blunt_log(x->f1 ,x->f2));
}

static void *b1_log_new(t_symbol *s ,int ac ,t_atom *av) {
	return (bop_new(b1_log_class ,s ,ac ,av));
}

/* --------------------- pow ------------------------------------- */
static t_class *b1_pow_class;

static void b1_pow_bang(t_bop *x) {
	outlet_float(x->bl.obj.ob_outlet ,blunt_pow(x->f1 ,x->f2));
}

static void *b1_pow_new(t_symbol *s ,int ac ,t_atom *av) {
	return (bop_new(b1_pow_class ,s ,ac ,av));
}

/* --------------------- max ------------------------------------- */
static t_class *b1_max_class;

static void b1_max_bang(t_bop *x) {
	outlet_float(x->bl.obj.ob_outlet ,blunt_max(x->f1 ,x->f2));
}

static void *b1_max_new(t_symbol *s ,int ac ,t_atom *av) {
	return (bop_new(b1_max_class ,s ,ac ,av));
}

/* --------------------- min ------------------------------------- */
static t_class *b1_min_class;

static void b1_min_bang(t_bop *x) {
	outlet_float(x->bl.obj.ob_outlet ,blunt_min(x->f1 ,x->f2));
}

static void *b1_min_new(t_symbol *s ,int ac ,t_atom *av) {
	return (bop_new(b1_min_class ,s ,ac ,av));
}

/* --------------- binop2: == ,!= ,> ,< ,>= ,<=. ----------------- */

/* --------------------- == -------------------------------------- */
static t_class *b2_ee_class;

static void b2_ee_bang(t_bop *x) {
	outlet_float(x->bl.obj.ob_outlet ,blunt_ee(x->f1 ,x->f2));
}

static void *b2_ee_new(t_symbol *s ,int ac ,t_atom *av) {
	return (bop_new(b2_ee_class ,s ,ac ,av));
}

/* --------------------- != -------------------------------------- */
static t_class *b2_ne_class;

static void b2_ne_bang(t_bop *x) {
	outlet_float(x->bl.obj.ob_outlet ,blunt_ne(x->f1 ,x->f2));
}

static void *b2_ne_new(t_symbol *s ,int ac ,t_atom *av) {
	return (bop_new(b2_ne_class ,s ,ac ,av));
}

/* --------------------- > --------------------------------------- */
static t_class *b2_gt_class;

static void b2_gt_bang(t_bop *x) {
	outlet_float(x->bl.obj.ob_outlet ,blunt_gt(x->f1 ,x->f2));
}

static void *b2_gt_new(t_symbol *s ,int ac ,t_atom *av) {
	return (bop_new(b2_gt_class ,s ,ac ,av));
}

/* --------------------- < --------------------------------------- */
static t_class *b2_lt_class;

static void b2_lt_bang(t_bop *x) {
	outlet_float(x->bl.obj.ob_outlet ,blunt_lt(x->f1 ,x->f2));
}

static void *b2_lt_new(t_symbol *s ,int ac ,t_atom *av) {
	return (bop_new(b2_lt_class ,s ,ac ,av));
}

/* --------------------- >= -------------------------------------- */
static t_class *b2_ge_class;

static void b2_ge_bang(t_bop *x) {
	outlet_float(x->bl.obj.ob_outlet ,blunt_ge(x->f1 ,x->f2));
}

static void *b2_ge_new(t_symbol *s ,int ac ,t_atom *av) {
	return (bop_new(b2_ge_class ,s ,ac ,av));
}

/* --------------------- <= -------------------------------------- */
static t_class *b2_le_class;

static void b2_le_bang(t_bop *x) {
	outlet_float(x->bl.obj.ob_outlet ,blunt_le(x->f1 ,x->f2));
}

static void *b2_le_new(t_symbol *s ,int ac ,t_atom *av) {
	return (bop_new(b2_le_class ,s ,ac ,av));
}

/* ------- binop3: & ,| ,&& ,|| ,<< ,>> ,^ ,% ,mod ,div ---------- */

/* --------------------- & --------------------------------------- */
static t_class *b3_ba_class;

static void b3_ba_bang(t_bop *x) {
	outlet_float(x->bl.obj.ob_outlet ,blunt_ba(x->f1 ,x->f2));
}

static void *b3_ba_new(t_symbol *s ,int ac ,t_atom *av) {
	return (bop_new(b3_ba_class ,s ,ac ,av));
}

/* --------------------- && -------------------------------------- */
static t_class *b3_la_class;

static void b3_la_bang(t_bop *x) {
	outlet_float(x->bl.obj.ob_outlet ,blunt_la(x->f1 ,x->f2));
}

static void *b3_la_new(t_symbol *s ,int ac ,t_atom *av) {
	return (bop_new(b3_la_class ,s ,ac ,av));
}

/* --------------------- | --------------------------------------- */
static t_class *b3_bo_class;

static void b3_bo_bang(t_bop *x) {
	outlet_float(x->bl.obj.ob_outlet ,blunt_bo(x->f1 ,x->f2));
}

static void *b3_bo_new(t_symbol *s ,int ac ,t_atom *av) {
	return (bop_new(b3_bo_class ,s ,ac ,av));
}

/* --------------------- || -------------------------------------- */
static t_class *b3_lo_class;

static void b3_lo_bang(t_bop *x) {
	outlet_float(x->bl.obj.ob_outlet ,blunt_lo(x->f1 ,x->f2));
}

static void *b3_lo_new(t_symbol *s ,int ac ,t_atom *av) {
	return (bop_new(b3_lo_class ,s ,ac ,av));
}

/* --------------------- << -------------------------------------- */
static t_class *b3_ls_class;

static void b3_ls_bang(t_bop *x) {
	outlet_float(x->bl.obj.ob_outlet ,blunt_ls(x->f1 ,x->f2));
}

static void *b3_ls_new(t_symbol *s ,int ac ,t_atom *av) {
	return (bop_new(b3_ls_class ,s ,ac ,av));
}

/* --------------------- >> -------------------------------------- */
static t_class *b3_rs_class;

static void b3_rs_bang(t_bop *x) {
	outlet_float(x->bl.obj.ob_outlet ,blunt_rs(x->f1 ,x->f2));
}

static void *b3_rs_new(t_symbol *s ,int ac ,t_atom *av) {
	return (bop_new(b3_rs_class ,s ,ac ,av));
}

/* --------------------- % --------------------------------------- */
static t_class *b3_pc_class;

static void b3_pc_bang(t_bop *x) {
	outlet_float(x->bl.obj.ob_outlet ,blunt_pc(x->f1 ,x->f2));
}

static void *b3_pc_new(t_symbol *s ,int ac ,t_atom *av) {
	return (bop_new(b3_pc_class ,s ,ac ,av));
}

/* --------------------- mod ------------------------------------- */
static t_class *b3_mod_class;

static void b3_mod_bang(t_bop *x) {
	outlet_float(x->bl.obj.ob_outlet ,blunt_mod(x->f1 ,x->f2));
}

static void *b3_mod_new(t_symbol *s ,int ac ,t_atom *av) {
	return (bop_new(b3_mod_class ,s ,ac ,av));
}

/* --------------------- div ------------------------------------- */
static t_class *b3_div_class;

static void b3_div_bang(t_bop *x) {
	outlet_float(x->bl.obj.ob_outlet ,blunt_divm(x->f1 ,x->f2));
}

static void *b3_div_new(t_symbol *s ,int ac ,t_atom *av) {
	return (bop_new(b3_div_class ,s ,ac ,av));
}

/* --------------------- ^ --------------------------------------- */
static t_class *b3_xor_class;

static void b3_xor_bang(t_bop *x) {
	outlet_float(x->bl.obj.ob_outlet ,blunt_xor(x->f1 ,x->f2));
}

static void *b3_xor_new(t_symbol *s ,int ac ,t_atom *av) {
	return (bop_new(b3_xor_class ,s ,ac ,av));
}

/* --------------------- f% --------------------------------------- */
static t_class *b3_fpc_class;

static void b3_fpc_bang(t_bop *x) {
	outlet_float(x->bl.obj.ob_outlet ,blunt_fpc(x->f1 ,x->f2));
}

static void *b3_fpc_new(t_symbol *s ,int ac ,t_atom *av) {
	return (bop_new(b3_fpc_class ,s ,ac ,av));
}


/* -------------------------- bang ------------------------------ */
static t_class *bng_class;

typedef struct {
	t_blunt bl;
} t_bng;

static void b_bang(t_bng *x) {
	outlet_bang(x->bl.obj.ob_outlet);
}

static void *b_new(t_symbol *s ,int ac ,t_atom *av) {
	t_bng *x = (t_bng *)pd_new(bng_class);
	outlet_new(&x->bl.obj ,&s_bang);
	if (ac && av->a_type == A_SYMBOL)
	{	const char *c = av->a_w.w_symbol->s_name;
		 x->bl.loadbang = (c[strlen(c)-1] == '!') ? 1 : 0;   }
	else x->bl.loadbang = 0;
	return (x);
}

static void bng_setup(void) {
	bng_class = class_new(gensym("b") ,(t_newmethod)b_new ,0
		,sizeof(t_bng) ,0 ,A_GIMME ,0);
	class_addcreator((t_newmethod)b_new ,gensym("`b") ,A_GIMME ,0);
	class_addbang     (bng_class ,b_bang);
	class_addfloat    (bng_class ,b_bang);
	class_addsymbol   (bng_class ,b_bang);
	class_addlist     (bng_class ,b_bang);
	class_addanything (bng_class ,b_bang);
	class_addmethod(bng_class ,(t_method)blunt_loadbang
		,gensym("loadbang") ,A_DEFFLOAT ,0);
	class_sethelpsymbol(bng_class ,gensym("blunt"));
}


/* -------------------------- symbol ------------------------------ */
static t_class *sym_class;

typedef struct {
	t_blunt bl;
	t_symbol *sym;
} t_sym;

static void sym_bang(t_sym *x) {
	outlet_symbol(x->bl.obj.ob_outlet ,x->sym);
}

static void sym_symbol(t_sym *x ,t_symbol *s) {
	outlet_symbol(x->bl.obj.ob_outlet ,x->sym = s);
}

static void sym_anything(t_sym *x ,t_symbol *s ,int ac ,t_atom *av) {
	outlet_symbol(x->bl.obj.ob_outlet ,x->sym = s);
}

static void sym_list(t_sym *x ,t_symbol *s ,int ac ,t_atom *av) {
	if (!ac)
		sym_bang(x);
	else if (av->a_type == A_SYMBOL)
		sym_symbol(x ,av->a_w.w_symbol);
	else sym_anything(x ,s ,ac ,av);
}

static void *sym_new(t_symbol *s ,int ac ,t_atom *av) {
	t_sym *x = (t_sym *)pd_new(sym_class);
	outlet_new      (&x->bl.obj ,&s_symbol);
	symbolinlet_new (&x->bl.obj ,&x->sym);
	x->sym = (ac && av->a_type == A_SYMBOL) ? av->a_w.w_symbol : &s_;
	const char *c = s->s_name;
	x->bl.loadbang = (c[strlen(c)-1] == '!') ? 1 : 0;
	return (x);
}

void sym_setup(void) {
	sym_class = class_new(gensym("sym") ,(t_newmethod)sym_new ,0
		,sizeof(t_sym) ,0 ,A_GIMME ,0);
	class_addcreator((t_newmethod)sym_new ,gensym("sym!") ,A_GIMME ,0);
	class_addbang     (sym_class ,sym_bang);
	class_addsymbol   (sym_class ,sym_symbol);
	class_addanything (sym_class ,sym_anything);
	class_addmethod(sym_class ,(t_method)blunt_loadbang
		,gensym("loadbang") ,A_DEFFLOAT ,0);
	class_sethelpsymbol(sym_class ,gensym("blunt"));
}


void blunt_setup(void) {

	post("Blunt! version 1.3");

	/* ---------------- connectives --------------------- */

	i_class = class_new(gensym("i") ,(t_newmethod)i_new ,0
		,sizeof(t_num) ,0 ,A_GIMME ,0);
	class_addcreator((t_newmethod)i_new ,gensym("`i") ,A_GIMME ,0);
	class_addmethod(i_class ,(t_method)i_send ,gensym("send") ,A_SYMBOL ,0);
	class_addbang  (i_class ,i_bang);

	f_class = class_new(gensym("f") ,(t_newmethod)f_new ,0
		,sizeof(t_num) ,0 ,A_GIMME ,0);
	class_addcreator((t_newmethod)f_new ,gensym("`f") ,A_GIMME ,0);
	class_addmethod(f_class ,(t_method)f_send ,gensym("send") ,A_SYMBOL ,0);
	class_addbang  (f_class ,f_bang);
	class_addsymbol(f_class ,f_symbol);

	t_class *nums[] = { i_class ,f_class };

	int i = sizeof(nums) / sizeof*(nums);
	t_symbol *num_sym = gensym("blunt");
	while (i--)
	{	class_addfloat (nums[i] ,num_float);
		class_addmethod(nums[i] ,(t_method)blunt_loadbang
			,gensym("loadbang") ,A_DEFFLOAT ,0);
		class_sethelpsymbol(nums[i] ,num_sym);   }

	/* ------------------ binop1 ----------------------- */

	b1_plus_class  = class_new(gensym("+")   ,(t_newmethod)b1_plus_new  ,0
		,sizeof(t_bop) ,0 ,A_GIMME ,0);
	class_addcreator((t_newmethod)b1_plus_new  ,gensym("`+")   ,A_GIMME ,0);

	b1_minus_class = class_new(gensym("-")   ,(t_newmethod)b1_minus_new ,0
		,sizeof(t_bop) ,0 ,A_GIMME ,0);
	class_addcreator((t_newmethod)b1_minus_new ,gensym("`-")   ,A_GIMME ,0);

	b1_times_class = class_new(gensym("*")   ,(t_newmethod)b1_times_new ,0
		,sizeof(t_bop) ,0 ,A_GIMME ,0);
	class_addcreator((t_newmethod)b1_times_new ,gensym("`*")   ,A_GIMME ,0);

	b1_div_class   = class_new(gensym("/")   ,(t_newmethod)b1_div_new   ,0
		,sizeof(t_bop) ,0 ,A_GIMME ,0);
	class_addcreator((t_newmethod)b1_div_new   ,gensym("`/")   ,A_GIMME ,0);

	b1_log_class   = class_new(gensym("log") ,(t_newmethod)b1_log_new   ,0
		,sizeof(t_bop) ,0 ,A_GIMME ,0);
	class_addcreator((t_newmethod)b1_log_new   ,gensym("`log") ,A_GIMME ,0);

	b1_pow_class   = class_new(gensym("pow") ,(t_newmethod)b1_pow_new   ,0
		,sizeof(t_bop) ,0 ,A_GIMME ,0);
	class_addcreator((t_newmethod)b1_pow_new   ,gensym("`pow") ,A_GIMME ,0);

	b1_max_class   = class_new(gensym("max") ,(t_newmethod)b1_max_new   ,0
		,sizeof(t_bop) ,0 ,A_GIMME ,0);
	class_addcreator((t_newmethod)b1_max_new   ,gensym("`max") ,A_GIMME ,0);

	b1_min_class   = class_new(gensym("min") ,(t_newmethod)b1_min_new   ,0
		,sizeof(t_bop) ,0 ,A_GIMME ,0);
	class_addcreator((t_newmethod)b1_min_new   ,gensym("`min") ,A_GIMME ,0);

	/* ------------------ binop2 ----------------------- */

	b2_ee_class    = class_new(gensym("==")  ,(t_newmethod)b2_ee_new    ,0
		,sizeof(t_bop) ,0 ,A_GIMME ,0);
	class_addcreator((t_newmethod)b2_ee_new    ,gensym("`==")  ,A_GIMME ,0);

	b2_ne_class    = class_new(gensym("!=")  ,(t_newmethod)b2_ne_new    ,0
		,sizeof(t_bop) ,0 ,A_GIMME ,0);
	class_addcreator((t_newmethod)b2_ne_new    ,gensym("`!=")  ,A_GIMME ,0);

	b2_gt_class    = class_new(gensym(">")   ,(t_newmethod)b2_gt_new    ,0
		,sizeof(t_bop) ,0 ,A_GIMME ,0);
	class_addcreator((t_newmethod)b2_gt_new    ,gensym("`>")   ,A_GIMME ,0);

	b2_lt_class    = class_new(gensym("<")   ,(t_newmethod)b2_lt_new    ,0
		,sizeof(t_bop) ,0 ,A_GIMME ,0);
	class_addcreator((t_newmethod)b2_lt_new    ,gensym("`<")   ,A_GIMME ,0);

	b2_ge_class    = class_new(gensym(">=")  ,(t_newmethod)b2_ge_new    ,0
		,sizeof(t_bop) ,0 ,A_GIMME ,0);
	class_addcreator((t_newmethod)b2_ge_new    ,gensym("`>=")  ,A_GIMME ,0);

	b2_le_class    = class_new(gensym("<=")  ,(t_newmethod)b2_le_new    ,0
		,sizeof(t_bop) ,0 ,A_GIMME ,0);
	class_addcreator((t_newmethod)b2_le_new    ,gensym("`<=")  ,A_GIMME ,0);

	/* ------------------ binop3 ----------------------- */

	b3_ba_class    = class_new(gensym("&")   ,(t_newmethod)b3_ba_new    ,0
		,sizeof(t_bop) ,0 ,A_GIMME ,0);
	class_addcreator((t_newmethod)b3_ba_new    ,gensym("`&")   ,A_GIMME ,0);

	b3_la_class    = class_new(gensym("&&")  ,(t_newmethod)b3_la_new    ,0
		,sizeof(t_bop) ,0 ,A_GIMME ,0);
	class_addcreator((t_newmethod)b3_la_new    ,gensym("`&&")  ,A_GIMME ,0);

	b3_bo_class    = class_new(gensym("|")   ,(t_newmethod)b3_bo_new    ,0
		,sizeof(t_bop) ,0 ,A_GIMME ,0);
	class_addcreator((t_newmethod)b3_bo_new    ,gensym("`|")   ,A_GIMME ,0);

	b3_lo_class    = class_new(gensym("||")  ,(t_newmethod)b3_lo_new    ,0
		,sizeof(t_bop) ,0 ,A_GIMME ,0);
	class_addcreator((t_newmethod)b3_lo_new    ,gensym("`||")  ,A_GIMME ,0);

	b3_ls_class    = class_new(gensym("<<")  ,(t_newmethod)b3_ls_new    ,0
		,sizeof(t_bop) ,0 ,A_GIMME ,0);
	class_addcreator((t_newmethod)b3_ls_new    ,gensym("`<<")  ,A_GIMME ,0);

	b3_rs_class    = class_new(gensym(">>")  ,(t_newmethod)b3_rs_new    ,0
		,sizeof(t_bop) ,0 ,A_GIMME ,0);
	class_addcreator((t_newmethod)b3_rs_new    ,gensym("`>>")  ,A_GIMME ,0);

	b3_pc_class    = class_new(gensym("%")   ,(t_newmethod)b3_pc_new    ,0
		,sizeof(t_bop) ,0 ,A_GIMME ,0);
	class_addcreator((t_newmethod)b3_pc_new    ,gensym("`%")   ,A_GIMME ,0);

	b3_mod_class   = class_new(gensym("mod") ,(t_newmethod)b3_mod_new   ,0
		,sizeof(t_bop) ,0 ,A_GIMME ,0);
	class_addcreator((t_newmethod)b3_mod_new   ,gensym("`mod") ,A_GIMME ,0);

	b3_div_class   = class_new(gensym("div") ,(t_newmethod)b3_div_new   ,0
		,sizeof(t_bop) ,0 ,A_GIMME ,0);
	class_addcreator((t_newmethod)b3_div_new   ,gensym("`div") ,A_GIMME ,0);

	b3_xor_class   = class_new(gensym("^")   ,(t_newmethod)b3_xor_new   ,0
		,sizeof(t_bop) ,0 ,A_GIMME ,0);

	b3_fpc_class   = class_new(gensym("f%")  ,(t_newmethod)b3_fpc_new   ,0
		,sizeof(t_bop) ,0 ,A_GIMME ,0);

	t_class *bops[][24] =
	{	{	 b1_plus_class ,b1_minus_class ,b1_times_class ,b1_div_class
			,b1_log_class  ,b1_pow_class   ,b1_max_class   ,b1_min_class
			,b2_ee_class   ,b2_ne_class    ,b2_gt_class    ,b2_lt_class
			,b2_ge_class   ,b2_le_class    ,b3_ba_class    ,b3_la_class
			,b3_bo_class   ,b3_lo_class    ,b3_ls_class    ,b3_rs_class
			,b3_fpc_class  ,b3_pc_class    ,b3_mod_class   ,b3_div_class   }
		,{	 b3_xor_class   }   };

	t_bopmethod bangs[][24] =
	{	{	 b1_plus_bang ,b1_minus_bang ,b1_times_bang ,b1_div_bang
			,b1_log_bang  ,b1_pow_bang   ,b1_max_bang   ,b1_min_bang
			,b2_ee_bang   ,b2_ne_bang    ,b2_gt_bang    ,b2_lt_bang
			,b2_ge_bang   ,b2_le_bang    ,b3_ba_bang    ,b3_la_bang
			,b3_bo_bang   ,b3_lo_bang    ,b3_ls_bang    ,b3_rs_bang
			,b3_fpc_bang  ,b3_pc_bang    ,b3_mod_bang   ,b3_div_bang   }
		,{	 b3_xor_bang   }   };

	t_symbol *syms[] = { num_sym ,gensym("0x5e") };

	i = sizeof syms / sizeof *syms;
	while (i--)
	{	int max = sizeof(bops[i]) / sizeof*(bops[i]);
		for (int j=0; j < max; j++)
		{	if (!bops[i][j]) break;
			class_addbang  (bops[i][j] ,bangs[i][j]);
			class_addfloat (bops[i][j] ,bop_float);
			class_addmethod(bops[i][j] ,(t_method)bop_f1
				,gensym("f1")  ,A_FLOAT ,0);
			class_addmethod(bops[i][j] ,(t_method)bop_f2
				,gensym("f2")  ,A_FLOAT ,0);
			class_addmethod(bops[i][j] ,(t_method)bop_skip
				,gensym(".")   ,A_GIMME ,0);
			class_addmethod(bops[i][j] ,(t_method)bop_set
				,gensym("set") ,A_GIMME ,0);
			class_addmethod(bops[i][j] ,(t_method)blunt_loadbang
				,gensym("loadbang") ,A_DEFFLOAT ,0);
			class_sethelpsymbol(bops[i][j] ,syms[i]);   }   }

	/* hot & reverse binops */
	hotop_setup();
	revop_setup();
	bng_setup();
	sym_setup();
}
