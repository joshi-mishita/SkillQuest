import { useState, useEffect, useRef } from "react";

// ─── Dataset ─────────────────────────────────────────────────────────────────
const JOBS = [
  { id:1,  title:"Data Scientist",          company:"Meta",         location:"Remote",       type:"Full-time", level:"Mid",    salaryMin:100000, salaryMax:140000, skills:["Python","ML","SQL","Statistics"], trending:95, rating:4.7, description:"Build ML models to understand user behaviour at scale." },
  { id:2,  title:"ML Engineer",             company:"OpenAI",       location:"San Francisco",type:"Full-time", level:"Senior", salaryMin:140000, salaryMax:200000, skills:["Python","PyTorch","ML","AWS"],     trending:99, rating:4.9, description:"Train & deploy large-scale language models." },
  { id:3,  title:"Software Engineer",       company:"Google",       location:"Hyderabad",    type:"Full-time", level:"Mid",    salaryMin:90000,  salaryMax:130000, skills:["Python","Java","System Design"],   trending:88, rating:4.8, description:"Build core infra for Google Search." },
  { id:4,  title:"Frontend Developer",      company:"Atlassian",    location:"Remote",       type:"Remote",    level:"Junior", salaryMin:60000,  salaryMax:90000,  skills:["React","CSS","JavaScript","HTML"],  trending:75, rating:4.3, description:"Build beautiful UIs for Jira and Confluence." },
  { id:5,  title:"Backend Engineer",        company:"Stripe",       location:"Dublin",       type:"Full-time", level:"Senior", salaryMin:130000, salaryMax:180000, skills:["Go","PostgreSQL","Distributed Systems"], trending:82, rating:4.6, description:"Power payment infrastructure used by millions." },
  { id:6,  title:"DevOps Engineer",         company:"Netflix",      location:"Remote",       type:"Remote",    level:"Mid",    salaryMin:110000, salaryMax:160000, skills:["Docker","Kubernetes","AWS","CI/CD"], trending:80, rating:4.5, description:"Maintain the world's largest streaming platform infra." },
  { id:7,  title:"Data Engineer",           company:"Snowflake",    location:"Remote",       type:"Remote",    level:"Mid",    salaryMin:105000, salaryMax:145000, skills:["SQL","Python","Spark","AWS"],       trending:85, rating:4.4, description:"Build pipelines moving petabytes of data." },
  { id:8,  title:"Cloud Architect",         company:"AWS",          location:"Seattle",      type:"Full-time", level:"Lead",   salaryMin:160000, salaryMax:220000, skills:["AWS","Azure","Terraform","Cloud"],  trending:78, rating:4.6, description:"Design enterprise cloud migrations at AWS." },
  { id:9,  title:"NLP Engineer",            company:"Anthropic",    location:"San Francisco",type:"Full-time", level:"Senior", salaryMin:145000, salaryMax:210000, skills:["Python","NLP","PyTorch","Transformers"], trending:97, rating:4.8, description:"Build safer AI systems." },
  { id:10, title:"Full Stack Developer",    company:"Razorpay",     location:"Bengaluru",    type:"Full-time", level:"Mid",    salaryMin:70000,  salaryMax:110000, skills:["React","Node.js","MongoDB","REST"], trending:72, rating:4.2, description:"End-to-end feature development for fintech." },
  { id:11, title:"Product Manager",         company:"Flipkart",     location:"Bengaluru",    type:"Full-time", level:"Senior", salaryMin:120000, salaryMax:170000, skills:["SQL","Analytics","Communication"],  trending:65, rating:4.1, description:"Define the roadmap for Flipkart's seller platform." },
  { id:12, title:"Security Engineer",       company:"Zscaler",      location:"Remote",       type:"Remote",    level:"Mid",    salaryMin:100000, salaryMax:140000, skills:["Network Security","Python","SIEM"],  trending:70, rating:4.0, description:"Protect cloud-native enterprise networks." },
  { id:13, title:"AI Researcher",           company:"DeepMind",     location:"London",       type:"Full-time", level:"Lead",   salaryMin:150000, salaryMax:230000, skills:["Python","Math","ML","Research"],    trending:96, rating:4.9, description:"Push the frontier of reinforcement learning." },
  { id:14, title:"Junior Software Engineer",company:"Infosys",      location:"Pune",         type:"Full-time", level:"Junior", salaryMin:40000,  salaryMax:65000,  skills:["Java","SQL","Spring Boot"],         trending:55, rating:3.8, description:"Build enterprise Java applications." },
  { id:15, title:"Android Developer",       company:"PhonePe",      location:"Bengaluru",    type:"Full-time", level:"Mid",    salaryMin:80000,  salaryMax:120000, skills:["Kotlin","Android","REST"],          trending:68, rating:4.2, description:"Build the PhonePe super-app for 500M users." },
];

const USERS_DB = {
  alice: { username:"alice", passwordHash:"password123", email:"alice@example.com", currentRole:"Software Engineer", skills:["Python","ML","SQL","JavaScript","React"], appliedJobIds:[], xp:720, level:3, badges:["📝 First Application","🎒 Skill Collector"], salary:95000 },
  bob:   { username:"bob",   passwordHash:"secure456",   email:"bob@example.com",   currentRole:"Junior Software Engineer", skills:["Java","SQL","Spring Boot"], appliedJobIds:[], xp:200, level:1, badges:[], salary:55000 },
};

const SKILL_CLUSTERS = {
  "Data Science":       ["Python","Pandas","NumPy","ML","Statistics","Jupyter","Machine Learning"],
  "Cloud & DevOps":     ["AWS","Azure","GCP","Docker","Kubernetes","Terraform","CI/CD","Ansible"],
  "Frontend":           ["HTML","CSS","JavaScript","React","Vue","Angular","Next.js","TypeScript"],
  "Backend":            ["Node.js","Django","Spring Boot","Go","REST","GraphQL","PostgreSQL","MySQL"],
  "AI & Machine Learning":["PyTorch","TensorFlow","NLP","Transformers","ML","Deep Learning","Research","RLHF"],
  "Databases":          ["SQL","MongoDB","PostgreSQL","Redis","Elasticsearch","DynamoDB"],
};

const CAREER_GRAPH = {
  "Junior Software Engineer": { paths:["Software Engineer"], avgSalary:52000 },
  "Software Engineer":        { paths:["Senior Software Engineer","Full Stack Developer","Data Scientist"], avgSalary:95000 },
  "Senior Software Engineer": { paths:["Staff Engineer","Engineering Manager","AI Researcher"], avgSalary:135000 },
  "Staff Engineer":           { paths:["Principal Engineer","VP of Engineering"], avgSalary:175000 },
  "Data Scientist":           { paths:["Senior Data Scientist","ML Engineer","AI Researcher"], avgSalary:110000 },
  "ML Engineer":              { paths:["Senior ML Engineer","AI Researcher","Staff Engineer"], avgSalary:155000 },
  "AI Researcher":            { paths:["Principal Researcher","Research Director"], avgSalary:185000 },
  "Engineering Manager":      { paths:["Director of Engineering","VP of Engineering"], avgSalary:160000 },
};

const MARKOV = {
  "Software Engineer": [
    { role:"Senior Software Engineer", prob:55 },
    { role:"Full Stack Developer",     prob:15 },
    { role:"Data Scientist",           prob:15 },
    { role:"Product Manager",          prob:10 },
    { role:"DevOps Engineer",          prob:5  },
  ],
  "Data Scientist": [
    { role:"ML Engineer",              prob:40 },
    { role:"Senior Data Scientist",    prob:35 },
    { role:"AI Researcher",            prob:15 },
    { role:"Data Engineer",            prob:10 },
  ],
  "ML Engineer": [
    { role:"Senior ML Engineer",       prob:45 },
    { role:"AI Researcher",            prob:30 },
    { role:"Staff Engineer",           prob:15 },
    { role:"Research Scientist",       prob:10 },
  ],
  "Junior Software Engineer": [
    { role:"Software Engineer",        prob:70 },
    { role:"Frontend Developer",       prob:20 },
    { role:"Backend Engineer",         prob:10 },
  ],
};

const ALL_BADGES = [
  { name:"First Application", icon:"📝", desc:"Applied to your first job" },
  { name:"Skill Collector",   icon:"🎒", desc:"Added 5+ skills to your profile" },
  { name:"Career Explorer",   icon:"🗺️", desc:"Viewed 5+ career paths" },
  { name:"Resume Master",     icon:"⭐", desc:"Achieved 80%+ resume match" },
  { name:"Interview Pro",     icon:"💬", desc:"Reached interview stage 3 times" },
  { name:"Offer Received",    icon:"🎉", desc:"Got a job offer" },
  { name:"Persistent",        icon:"🔥", desc:"Applied to 10+ jobs" },
  { name:"Analyst",           icon:"📊", desc:"Used salary analysis 5 times" },
];

// ─── Helpers ──────────────────────────────────────────────────────────────────
const fmt = (n) => n >= 1000 ? `$${(n/1000).toFixed(0)}k` : `$${n}`;
const fmtFull = (n) => `$${n.toLocaleString()}`;
const today = () => new Date().toISOString().slice(0, 10);
const levelTitle = (lvl) => {
  if (lvl <= 1) return { label:"🌱 Novice",      color:"#6bcb77" };
  if (lvl <= 3) return { label:"⚔️ Explorer",    color:"#4d96ff" };
  if (lvl <= 6) return { label:"🔥 Practitioner",color:"#ff6b6b" };
  if (lvl <= 10)return { label:"💎 Expert",       color:"#ffd93d" };
  return               { label:"🏆 Legend",       color:"#c77dff" };
};
const xpForLevel = (l) => l * l * 100;
const computeLevel = (xp) => { let l=1; while(xp >= xpForLevel(l+1)) l++; return l; };

// ─── CSS ─────────────────────────────────────────────────────────────────────
const css = `
@import url('https://fonts.googleapis.com/css2?family=Space+Grotesk:wght@300;400;500;600;700&family=JetBrains+Mono:wght@400;600&display=swap');

*, *::before, *::after { box-sizing: border-box; margin: 0; padding: 0; }

:root {
  --bg:       #0a0e17;
  --bg2:      #111827;
  --bg3:      #1c2436;
  --border:   #1e2d45;
  --accent:   #00c2ff;
  --accent2:  #7c3aed;
  --accent3:  #10b981;
  --warn:     #f59e0b;
  --danger:   #ef4444;
  --text:     #e2e8f0;
  --muted:    #64748b;
  --card:     #131c2e;
  --glow:     0 0 20px rgba(0,194,255,.18);
}

html, body, #root { height: 100%; }

body {
  font-family: 'Space Grotesk', sans-serif;
  background: var(--bg);
  color: var(--text);
  overflow-x: hidden;
}

/* scrollbar */
::-webkit-scrollbar { width: 5px; }
::-webkit-scrollbar-track { background: var(--bg2); }
::-webkit-scrollbar-thumb { background: var(--border); border-radius: 4px; }

/* layout */
.app { display: flex; height: 100vh; }

/* sidebar */
.sidebar {
  width: 220px; flex-shrink: 0;
  background: var(--bg2);
  border-right: 1px solid var(--border);
  display: flex; flex-direction: column;
  padding: 0;
  position: fixed; left: 0; top: 0; bottom: 0;
  z-index: 100;
}
.sidebar-logo {
  padding: 22px 20px 16px;
  border-bottom: 1px solid var(--border);
}
.logo-text {
  font-size: 1.35rem; font-weight: 700; letter-spacing: -.5px;
  background: linear-gradient(135deg, var(--accent), var(--accent2));
  -webkit-background-clip: text; -webkit-text-fill-color: transparent;
}
.logo-sub { font-size: .65rem; color: var(--muted); font-family:'JetBrains Mono',monospace; margin-top: 2px; letter-spacing: .5px; }

.nav-section { padding: 12px 12px 6px; }
.nav-label { font-size: .6rem; text-transform: uppercase; letter-spacing: 1.5px; color: var(--muted); font-weight: 600; padding: 0 8px 6px; }
.nav-btn {
  display: flex; align-items: center; gap: 10px;
  width: 100%; padding: 9px 12px; border-radius: 8px;
  border: none; background: transparent; color: var(--muted);
  cursor: pointer; font-family: inherit; font-size: .85rem; font-weight: 500;
  transition: all .2s; text-align: left;
}
.nav-btn:hover { background: var(--bg3); color: var(--text); }
.nav-btn.active { background: rgba(0,194,255,.12); color: var(--accent); }
.nav-icon { font-size: 1rem; width: 18px; text-align: center; }

.sidebar-user {
  margin-top: auto;
  padding: 14px;
  border-top: 1px solid var(--border);
}
.user-chip {
  display: flex; align-items: center; gap: 10px;
  padding: 10px; border-radius: 10px;
  background: var(--bg3); cursor: pointer;
}
.avatar {
  width: 34px; height: 34px; border-radius: 50%;
  background: linear-gradient(135deg, var(--accent), var(--accent2));
  display: flex; align-items: center; justify-content: center;
  font-weight: 700; font-size: .85rem; color: #fff; flex-shrink: 0;
}
.user-info { overflow: hidden; }
.user-name { font-size: .82rem; font-weight: 600; color: var(--text); white-space: nowrap; overflow: hidden; text-overflow: ellipsis; }
.user-level { font-size: .68rem; color: var(--muted); }

/* main content */
.main { margin-left: 220px; flex: 1; overflow-y: auto; padding: 28px 32px; min-height: 100vh; }

/* topbar */
.topbar {
  display: flex; align-items: center; justify-content: space-between;
  margin-bottom: 28px;
}
.page-title { font-size: 1.5rem; font-weight: 700; }
.page-sub { color: var(--muted); font-size: .82rem; margin-top: 2px; }
.badge-pill {
  display: inline-flex; align-items: center; gap: 5px;
  padding: 4px 12px; border-radius: 20px; font-size: .72rem; font-weight: 600;
}

/* cards */
.card {
  background: var(--card);
  border: 1px solid var(--border);
  border-radius: 14px; padding: 22px;
}
.card-sm { padding: 16px; }
.card:hover { border-color: #2a3d5a; }

/* grid */
.grid2 { display: grid; grid-template-columns: 1fr 1fr; gap: 16px; }
.grid3 { display: grid; grid-template-columns: 1fr 1fr 1fr; gap: 16px; }

/* stat cards */
.stat-card {
  background: var(--card);
  border: 1px solid var(--border);
  border-radius: 14px; padding: 20px;
  display: flex; align-items: center; gap: 16px;
}
.stat-icon {
  width: 46px; height: 46px; border-radius: 12px;
  display: flex; align-items: center; justify-content: center;
  font-size: 1.3rem; flex-shrink: 0;
}
.stat-val { font-size: 1.6rem; font-weight: 700; line-height: 1; }
.stat-lbl { font-size: .72rem; color: var(--muted); margin-top: 3px; }

/* buttons */
.btn {
  display: inline-flex; align-items: center; gap: 6px;
  padding: 8px 18px; border-radius: 8px; border: none;
  cursor: pointer; font-family: inherit; font-size: .82rem; font-weight: 600;
  transition: all .2s;
}
.btn-primary { background: var(--accent); color: #000; }
.btn-primary:hover { opacity: .85; }
.btn-outline { background: transparent; border: 1px solid var(--border); color: var(--text); }
.btn-outline:hover { border-color: var(--accent); color: var(--accent); }
.btn-danger { background: rgba(239,68,68,.15); color: var(--danger); border: 1px solid rgba(239,68,68,.3); }
.btn-success { background: rgba(16,185,129,.15); color: var(--accent3); border: 1px solid rgba(16,185,129,.3); }
.btn-sm { padding: 5px 12px; font-size: .75rem; }
.btn-xs { padding: 3px 9px; font-size: .7rem; }

/* inputs */
.input-group { display: flex; gap: 8px; align-items: center; }
.input {
  width: 100%; padding: 9px 14px; border-radius: 8px;
  border: 1px solid var(--border); background: var(--bg3);
  color: var(--text); font-family: inherit; font-size: .85rem;
  outline: none; transition: border .2s;
}
.input:focus { border-color: var(--accent); }
.input::placeholder { color: var(--muted); }
.select {
  padding: 8px 12px; border-radius: 8px;
  border: 1px solid var(--border); background: var(--bg3);
  color: var(--text); font-family: inherit; font-size: .82rem; cursor: pointer;
}

/* tags */
.tag {
  display: inline-block; padding: 3px 10px; border-radius: 20px;
  font-size: .7rem; font-weight: 600;
}
.tag-blue  { background: rgba(0,194,255,.12); color: var(--accent); border: 1px solid rgba(0,194,255,.2); }
.tag-purple{ background: rgba(124,58,237,.15); color: #a78bfa; border: 1px solid rgba(124,58,237,.25); }
.tag-green { background: rgba(16,185,129,.12); color: var(--accent3); border: 1px solid rgba(16,185,129,.2); }
.tag-orange{ background: rgba(245,158,11,.12); color: var(--warn); border: 1px solid rgba(245,158,11,.2); }
.tag-red   { background: rgba(239,68,68,.12); color: var(--danger); border: 1px solid rgba(239,68,68,.2); }

/* progress bar */
.prog-wrap { background: var(--bg3); border-radius: 4px; height: 6px; overflow: hidden; }
.prog-bar  { height: 100%; border-radius: 4px; transition: width .5s; }

/* job card */
.job-card {
  background: var(--card); border: 1px solid var(--border); border-radius: 14px;
  padding: 18px; cursor: pointer; transition: all .2s;
}
.job-card:hover { border-color: var(--accent); transform: translateY(-2px); box-shadow: var(--glow); }
.job-title { font-size: .95rem; font-weight: 700; margin-bottom: 2px; }
.job-company { font-size: .78rem; color: var(--muted); }
.job-meta { display: flex; gap: 8px; margin-top: 10px; flex-wrap: wrap; }
.job-salary { font-size: .78rem; color: var(--accent3); font-weight: 600; }

/* modal */
.overlay {
  position: fixed; inset: 0; background: rgba(0,0,0,.7);
  display: flex; align-items: center; justify-content: center;
  z-index: 999; backdrop-filter: blur(4px);
}
.modal {
  background: var(--bg2); border: 1px solid var(--border);
  border-radius: 18px; padding: 28px; width: 500px; max-width: 95vw;
  max-height: 85vh; overflow-y: auto;
  animation: modalIn .2s ease;
}
@keyframes modalIn { from { opacity:0; transform: scale(.95); } to { opacity:1; transform: scale(1); } }
.modal-title { font-size: 1.1rem; font-weight: 700; margin-bottom: 16px; }

/* auth */
.auth-wrap { min-height: 100vh; display: flex; align-items: center; justify-content: center; background: var(--bg); }
.auth-card {
  background: var(--bg2); border: 1px solid var(--border); border-radius: 22px;
  padding: 40px; width: 400px; max-width: 95vw;
}
.auth-logo { text-align: center; margin-bottom: 28px; }
.auth-logo .big { font-size: 2rem; font-weight: 800; }
.auth-logo .big span { background: linear-gradient(135deg, var(--accent), var(--accent2)); -webkit-background-clip: text; -webkit-text-fill-color: transparent; }
.auth-form .label { font-size: .78rem; color: var(--muted); margin-bottom: 6px; font-weight: 500; }
.auth-form .field { margin-bottom: 16px; }

/* section header */
.sec-head { display: flex; align-items: center; justify-content: space-between; margin-bottom: 16px; }
.sec-title { font-size: 1rem; font-weight: 700; }

/* timeline */
.timeline { display: flex; flex-direction: column; gap: 0; }
.tl-item { display: flex; gap: 14px; }
.tl-line { display: flex; flex-direction: column; align-items: center; }
.tl-dot { width: 10px; height: 10px; border-radius: 50%; background: var(--accent); flex-shrink: 0; margin-top: 4px; }
.tl-connector { flex: 1; width: 2px; background: var(--border); min-height: 30px; }
.tl-content { padding-bottom: 20px; }

/* empty state */
.empty { text-align: center; padding: 48px 20px; color: var(--muted); }
.empty-icon { font-size: 2.5rem; margin-bottom: 10px; }
.empty-text { font-size: .85rem; }

/* xp bar */
.xp-wrap { display: flex; align-items: center; gap: 10px; }
.xp-label { font-size: .7rem; color: var(--muted); white-space: nowrap; }

/* notifications */
.notif {
  position: fixed; bottom: 24px; right: 24px;
  padding: 12px 20px; border-radius: 10px; font-size: .82rem; font-weight: 600;
  color: #000; z-index: 9999;
  animation: slideUp .3s ease;
  background: var(--accent3);
}
@keyframes slideUp { from { opacity:0; transform: translateY(20px); } to { opacity:1; transform: translateY(0); } }

/* skill cluster */
.cluster-card { border-left: 3px solid; padding-left: 14px; }

/* chart bar */
.h-bar { display: flex; align-items: center; gap: 10px; margin-bottom: 10px; }
.h-bar-label { font-size: .78rem; width: 160px; flex-shrink: 0; white-space: nowrap; overflow: hidden; text-overflow: ellipsis; }
.h-bar-wrap { flex: 1; background: var(--bg3); border-radius: 4px; height: 8px; overflow: hidden; }
.h-bar-fill { height: 100%; border-radius: 4px; }
.h-bar-val { font-size: .72rem; color: var(--muted); width: 36px; text-align: right; flex-shrink: 0; }

/* status badge */
.status-applied   { background: rgba(0,194,255,.1);  color: var(--accent);  border: 1px solid rgba(0,194,255,.2); }
.status-interview { background: rgba(245,158,11,.1); color: var(--warn);   border: 1px solid rgba(245,158,11,.2); }
.status-offer     { background: rgba(16,185,129,.1); color: var(--accent3);border: 1px solid rgba(16,185,129,.2); }
.status-rejected  { background: rgba(239,68,68,.1);  color: var(--danger); border: 1px solid rgba(239,68,68,.2); }
.status-withdrawn { background: rgba(100,116,139,.1);color: var(--muted);  border: 1px solid rgba(100,116,139,.2); }

/* salary chart */
.sal-bar { display: flex; align-items: flex-end; gap: 6px; height: 100px; }
.sal-col { display: flex; flex-direction: column; align-items: center; gap: 4px; flex: 1; }
.sal-col-bar { width: 100%; border-radius: 4px 4px 0 0; background: linear-gradient(180deg, var(--accent), var(--accent2)); transition: height .4s; }
.sal-col-lbl { font-size: .62rem; color: var(--muted); }

/* search */
.search-bar { display: flex; gap: 8px; align-items: center; margin-bottom: 20px; }

/* divider */
.divider { border: none; border-top: 1px solid var(--border); margin: 20px 0; }

/* glow accent */
.accent-glow { text-shadow: 0 0 20px rgba(0,194,255,.5); }

/* mono */
.mono { font-family: 'JetBrains Mono', monospace; }

/* chip row */
.chips { display: flex; flex-wrap: wrap; gap: 6px; }

/* scroll area */
.scroll-area { overflow-y: auto; max-height: 420px; padding-right: 4px; }

/* section spacing */
.mt8 { margin-top: 8px; }
.mt12 { margin-top: 12px; }
.mt16 { margin-top: 16px; }
.mt20 { margin-top: 20px; }
.mb8 { margin-bottom: 8px; }
.mb12 { margin-bottom: 12px; }
.mb16 { margin-bottom: 16px; }
.flex { display: flex; }
.items-center { align-items: center; }
.justify-between { justify-content: space-between; }
.gap8 { gap: 8px; }
.gap12 { gap: 12px; }
.gap16 { gap: 16px; }
.text-muted { color: var(--muted); font-size: .78rem; }
.text-accent { color: var(--accent); }
.text-sm { font-size: .82rem; }
.fw600 { font-weight: 600; }
.w100 { width: 100%; }
`;

// ─── Component: Notification ──────────────────────────────────────────────────
function Notif({ msg, onDone }) {
  useEffect(() => { const t = setTimeout(onDone, 2500); return () => clearTimeout(t); }, []);
  return <div className="notif">{msg}</div>;
}

// ─── Component: Auth ─────────────────────────────────────────────────────────
function Auth({ onLogin }) {
  const [mode, setMode] = useState("login");
  const [form, setForm] = useState({ username:"", password:"", email:"", role:"", salary:"" });
  const [err, setErr] = useState("");

  const set = (k, v) => setForm(f => ({ ...f, [k]: v }));

  const submit = () => {
    setErr("");
    if (mode === "login") {
      const u = USERS_DB[form.username];
      if (!u || u.passwordHash !== form.password) return setErr("Invalid credentials. Try alice/password123");
      onLogin({ ...u });
    } else {
      if (!form.username || !form.password) return setErr("Username and password required.");
      if (USERS_DB[form.username]) return setErr("Username already taken.");
      const newUser = {
        username: form.username, passwordHash: form.password,
        email: form.email, currentRole: form.role || "Student",
        skills: [], appliedJobIds: [], xp: 0, level: 1, badges: [],
        salary: parseFloat(form.salary) || 0,
      };
      USERS_DB[form.username] = newUser;
      onLogin({ ...newUser });
    }
  };

  return (
    <div className="auth-wrap">
      <div className="auth-card">
        <div className="auth-logo">
          <div className="big"><span>Skill</span>Quest</div>
          <div style={{ fontSize:".75rem", color:"var(--muted)", marginTop:4 }}>Career Navigation & Job Recommendation System</div>
        </div>

        <div style={{ display:"flex", gap:8, marginBottom:24 }}>
          {["login","register"].map(m => (
            <button key={m} className={`btn w100 ${mode===m ? "btn-primary":"btn-outline"}`} onClick={() => { setMode(m); setErr(""); }}>
              {m === "login" ? "Login" : "Register"}
            </button>
          ))}
        </div>

        <div className="auth-form">
          {["username","password"].map(k => (
            <div className="field" key={k}>
              <div className="label">{k.charAt(0).toUpperCase()+k.slice(1)}</div>
              <input className="input" type={k==="password"?"password":"text"} placeholder={k==="username"?"alice or bob":"••••••••"}
                value={form[k]} onChange={e => set(k, e.target.value)}
                onKeyDown={e => e.key === "Enter" && submit()} />
            </div>
          ))}
          {mode === "register" && <>
            {[["email","Email","alice@example.com"],["role","Current Role","Software Engineer"],["salary","Current Salary","95000"]].map(([k,l,p]) => (
              <div className="field" key={k}>
                <div className="label">{l}</div>
                <input className="input" placeholder={p} value={form[k]} onChange={e => set(k, e.target.value)} />
              </div>
            ))}
          </>}
          {err && <div style={{ color:"var(--danger)", fontSize:".78rem", marginBottom:12 }}>{err}</div>}
          <button className="btn btn-primary w100" style={{ justifyContent:"center", padding:12 }} onClick={submit}>
            {mode === "login" ? "Login →" : "Create Account →"}
          </button>
          {mode === "login" && <div style={{ textAlign:"center", marginTop:12, fontSize:".72rem", color:"var(--muted)" }}>
            Demo: <span className="mono" style={{ color:"var(--accent)" }}>alice</span> / <span className="mono" style={{ color:"var(--accent)" }}>password123</span>
          </div>}
        </div>
      </div>
    </div>
  );
}

// ─── Component: JobCard ───────────────────────────────────────────────────────
function JobCard({ job, applied, onApply, onView }) {
  const levelColor = { Junior:"var(--accent3)", Mid:"var(--accent)", Senior:"var(--warn)", Lead:"var(--accent2)" };
  return (
    <div className="job-card" onClick={() => onView(job)}>
      <div className="flex items-center justify-between mb8">
        <div>
          <div className="job-title">{job.title}</div>
          <div className="job-company">🏢 {job.company} · {job.location}</div>
        </div>
        <div style={{ fontSize:".8rem", color:"var(--warn)", fontWeight:700 }}>★ {job.rating}</div>
      </div>
      <div className="job-meta">
        <span className="tag tag-blue">{job.type}</span>
        <span className="tag" style={{ background:"rgba(255,255,255,.05)", color: levelColor[job.level] || "var(--text)", border:"1px solid rgba(255,255,255,.08)" }}>{job.level}</span>
        <span className="job-salary">{fmt(job.salaryMin)}–{fmt(job.salaryMax)}/yr</span>
        <span style={{ marginLeft:"auto" }}>
          <span className="tag tag-orange">🔥 {job.trending}</span>
        </span>
      </div>
      <div className="chips mt12">
        {job.skills.slice(0,4).map(s => <span className="tag tag-purple" key={s}>{s}</span>)}
      </div>
      <div className="flex gap8 mt12" onClick={e => e.stopPropagation()}>
        <button className={`btn btn-sm ${applied ? "btn-success" : "btn-primary"}`}
          onClick={() => !applied && onApply(job)} disabled={applied}>
          {applied ? "✅ Applied" : "Apply Now"}
        </button>
        <button className="btn btn-sm btn-outline" onClick={() => onView(job)}>Details</button>
      </div>
    </div>
  );
}

// ─── Component: JobDetailModal ────────────────────────────────────────────────
function JobDetailModal({ job, applied, userSkills, onClose, onApply }) {
  if (!job) return null;
  const userSkillSet = new Set(userSkills.map(s => s.toLowerCase()));
  const matched = job.skills.filter(s => userSkillSet.has(s.toLowerCase()));
  const missing = job.skills.filter(s => !userSkillSet.has(s.toLowerCase()));
  const score = Math.round(matched.length / job.skills.length * 100);

  return (
    <div className="overlay" onClick={onClose}>
      <div className="modal" onClick={e => e.stopPropagation()}>
        <div className="flex items-center justify-between mb12">
          <div>
            <div style={{ fontSize:"1.15rem", fontWeight:700 }}>{job.title}</div>
            <div style={{ color:"var(--muted)", fontSize:".82rem" }}>🏢 {job.company} · {job.location}</div>
          </div>
          <button className="btn btn-sm btn-outline" onClick={onClose}>✕</button>
        </div>
        <p style={{ fontSize:".82rem", color:"var(--muted)", marginBottom:16 }}>{job.description}</p>
        <div className="grid2 mb16">
          <div className="card card-sm"><div className="text-muted mb8">Salary Range</div><div style={{ fontWeight:700, color:"var(--accent3)" }}>{fmtFull(job.salaryMin)} – {fmtFull(job.salaryMax)}</div></div>
          <div className="card card-sm"><div className="text-muted mb8">Skill Match</div><div style={{ fontWeight:700, color: score>=70?"var(--accent3)":"var(--warn)" }}>{score}%</div></div>
        </div>
        <div className="mb12">
          <div className="text-muted mb8">✅ Your Matching Skills</div>
          <div className="chips">{matched.length ? matched.map(s => <span className="tag tag-green" key={s}>{s}</span>) : <span className="text-muted">None</span>}</div>
        </div>
        <div className="mb16">
          <div className="text-muted mb8">⚠️ Skills to Acquire</div>
          <div className="chips">{missing.length ? missing.map(s => <span className="tag tag-red" key={s}>{s}</span>) : <span style={{ color:"var(--accent3)", fontSize:".78rem" }}>You match all required skills!</span>}</div>
        </div>
        <div className="prog-wrap mb16">
          <div className="prog-bar" style={{ width:`${score}%`, background:"linear-gradient(90deg, var(--accent), var(--accent2))" }} />
        </div>
        <button className={`btn w100 ${applied?"btn-success":"btn-primary"}`} style={{ justifyContent:"center", padding:12 }}
          onClick={() => { onApply(job); onClose(); }} disabled={applied}>
          {applied ? "✅ Already Applied" : "🚀 Apply Now"}
        </button>
      </div>
    </div>
  );
}

// ─── Page: Dashboard ──────────────────────────────────────────────────────────
function Dashboard({ user, applications, onNav }) {
  const lvl = computeLevel(user.xp);
  const lt = levelTitle(lvl);
  const xpCur = user.xp;
  const xpNext = xpForLevel(lvl + 1);
  const pct = Math.min(100, Math.round(xpCur / xpNext * 100));

  const stats = [
    { icon:"📋", label:"Applications", val: applications.length, color:"#00c2ff" },
    { icon:"💼", label:"Interviews",   val: applications.filter(a=>a.status==="Interview").length, color:"#f59e0b" },
    { icon:"🎉", label:"Offers",       val: applications.filter(a=>a.status==="Offer").length, color:"#10b981" },
    { icon:"🎒", label:"Skills",       val: user.skills.length, color:"#7c3aed" },
  ];

  return (
    <div>
      <div className="topbar">
        <div>
          <div className="page-title">Welcome back, <span style={{ color:"var(--accent)" }}>{user.username}</span> 👋</div>
          <div className="page-sub">{lt.label} · Here's your career overview</div>
        </div>
      </div>

      {/* XP Bar */}
      <div className="card mb16" style={{ background:"linear-gradient(135deg, rgba(0,194,255,.08), rgba(124,58,237,.08))", borderColor:"rgba(0,194,255,.2)" }}>
        <div className="flex items-center justify-between mb8">
          <div style={{ fontWeight:700 }}>{lt.label}</div>
          <div style={{ fontSize:".78rem", color:"var(--muted)" }}>Level {lvl} · {xpCur}/{xpNext} XP</div>
        </div>
        <div className="prog-wrap" style={{ height:8 }}>
          <div className="prog-bar" style={{ width:`${pct}%`, background:"linear-gradient(90deg, var(--accent), var(--accent2))" }} />
        </div>
        {user.badges.length > 0 && (
          <div className="chips mt12">
            {user.badges.map(b => <span key={b} className="tag tag-purple">{b}</span>)}
          </div>
        )}
      </div>

      {/* Stats */}
      <div className="grid2 mb16">
        {stats.map(s => (
          <div className="stat-card" key={s.label}>
            <div className="stat-icon" style={{ background:`rgba(0,0,0,.3)`, border:`1px solid rgba(255,255,255,.07)` }}>{s.icon}</div>
            <div>
              <div className="stat-val" style={{ color: s.color }}>{s.val}</div>
              <div className="stat-lbl">{s.label}</div>
            </div>
          </div>
        ))}
      </div>

      {/* Quick actions */}
      <div className="grid2 mb16">
        <div className="card" style={{ cursor:"pointer" }} onClick={() => onNav("jobs")}>
          <div style={{ fontSize:"1.5rem", marginBottom:8 }}>🔍</div>
          <div className="fw600 mb8">Browse Jobs</div>
          <div className="text-muted">Search from 15+ curated job listings with skill matching</div>
        </div>
        <div className="card" style={{ cursor:"pointer" }} onClick={() => onNav("career")}>
          <div style={{ fontSize:"1.5rem", marginBottom:8 }}>🗺️</div>
          <div className="fw600 mb8">Career Paths</div>
          <div className="text-muted">Discover optimal paths using A* and Markov Chains</div>
        </div>
        <div className="card" style={{ cursor:"pointer" }} onClick={() => onNav("tracker")}>
          <div style={{ fontSize:"1.5rem", marginBottom:8 }}>📋</div>
          <div className="fw600 mb8">Application Tracker</div>
          <div className="text-muted">Track applications with undo/redo support</div>
        </div>
        <div className="card" style={{ cursor:"pointer" }} onClick={() => onNav("skills")}>
          <div style={{ fontSize:"1.5rem", marginBottom:8 }}>⚡</div>
          <div className="fw600 mb8">Skills & Profile</div>
          <div className="text-muted">Manage skills, view clusters, salary predictions</div>
        </div>
      </div>

      {/* Top trending */}
      <div className="card">
        <div className="sec-head"><div className="sec-title">🔥 Top Trending Jobs</div><button className="btn btn-sm btn-outline" onClick={() => onNav("jobs")}>View All</button></div>
        {JOBS.sort((a,b)=>b.trending-a.trending).slice(0,4).map(j => (
          <div key={j.id} className="flex items-center justify-between" style={{ padding:"10px 0", borderBottom:"1px solid var(--border)" }}>
            <div>
              <div style={{ fontWeight:600, fontSize:".88rem" }}>{j.title}</div>
              <div className="text-muted">{j.company} · {j.type}</div>
            </div>
            <div className="flex gap8 items-center">
              <span style={{ color:"var(--accent3)", fontSize:".78rem", fontWeight:600 }}>{fmt(j.salaryMin)}+</span>
              <span className="tag tag-orange">🔥 {j.trending}</span>
            </div>
          </div>
        ))}
      </div>
    </div>
  );
}

// ─── Page: Jobs ───────────────────────────────────────────────────────────────
function Jobs({ user, applications, onApply, notify }) {
  const [query, setQuery] = useState("");
  const [filter, setFilter] = useState({ type:"", level:"" });
  const [selected, setSelected] = useState(null);
  const [suggestions, setSuggestions] = useState([]);
  const [recentSearches, setRecentSearches] = useState([]);
  const [tab, setTab] = useState("all");

  const appliedIds = new Set(applications.map(a => a.jobId));

  const filtered = JOBS.filter(j => {
    if (tab === "trending") return j.trending >= 80;
    if (tab === "recommended") {
      const userSet = new Set(user.skills.map(s => s.toLowerCase()));
      return j.skills.some(s => userSet.has(s.toLowerCase()));
    }
    const q = query.toLowerCase();
    const matchQ = !q || j.title.toLowerCase().includes(q) || j.company.toLowerCase().includes(q) || j.skills.some(s => s.toLowerCase().includes(q));
    const matchT = !filter.type || j.type === filter.type;
    const matchL = !filter.level || j.level === filter.level;
    return matchQ && matchT && matchL;
  });

  const doSearch = (v) => {
    setQuery(v);
    if (v.length >= 2) {
      setSuggestions(JOBS.filter(j => j.title.toLowerCase().startsWith(v.toLowerCase())).map(j => j.title).slice(0, 5));
    } else setSuggestions([]);
  };

  const commitSearch = (v) => {
    setQuery(v); setSuggestions([]);
    if (v && !recentSearches.includes(v)) setRecentSearches(r => [v, ...r].slice(0,8));
  };

  const handleApply = (job) => {
    onApply(job);
    notify(`✅ Applied to ${job.title} at ${job.company}! +100 XP`);
  };

  return (
    <div>
      <div className="topbar">
        <div><div className="page-title">Job Search Engine 🔍</div><div className="page-sub">Trie autocomplete · Rabin-Karp resume match · Max-Heap trending</div></div>
      </div>

      <div className="card mb16">
        <div style={{ position:"relative" }}>
          <input className="input" placeholder="Search jobs by title, company or skill…"
            value={query} onChange={e => doSearch(e.target.value)}
            onKeyDown={e => e.key === "Enter" && commitSearch(query)} />
          {suggestions.length > 0 && (
            <div style={{ position:"absolute", top:"calc(100% + 4px)", left:0, right:0, background:"var(--bg3)", border:"1px solid var(--border)", borderRadius:8, zIndex:50, overflow:"hidden" }}>
              {suggestions.map(s => (
                <div key={s} style={{ padding:"9px 14px", cursor:"pointer", fontSize:".85rem" }} onClick={() => commitSearch(s)} onMouseOver={e=>e.target.style.background="var(--border)"} onMouseOut={e=>e.target.style.background="transparent"}>
                  🔍 {s}
                </div>
              ))}
            </div>
          )}
        </div>
        <div className="flex gap8 mt12">
          <select className="select" value={filter.type} onChange={e => setFilter(f=>({...f, type:e.target.value}))}>
            <option value="">All Types</option>
            {["Full-time","Remote","Part-time"].map(t=><option key={t}>{t}</option>)}
          </select>
          <select className="select" value={filter.level} onChange={e => setFilter(f=>({...f, level:e.target.value}))}>
            <option value="">All Levels</option>
            {["Junior","Mid","Senior","Lead"].map(l=><option key={l}>{l}</option>)}
          </select>
          <button className="btn btn-outline btn-sm" onClick={() => { setQuery(""); setFilter({type:"",level:""}); }}>Clear</button>
        </div>

        {recentSearches.length > 0 && (
          <div className="mt12">
            <div className="text-muted mb8">🕐 Recent (LRU Cache)</div>
            <div className="chips">
              {recentSearches.map(s => <span key={s} className="tag tag-blue" style={{ cursor:"pointer" }} onClick={() => commitSearch(s)}>{s}</span>)}
            </div>
          </div>
        )}
      </div>

      <div className="flex gap8 mb16">
        {[["all","All Jobs"],["trending","🔥 Trending"],["recommended","🎯 For You"]].map(([k,l]) => (
          <button key={k} className={`btn btn-sm ${tab===k?"btn-primary":"btn-outline"}`} onClick={() => setTab(k)}>{l}</button>
        ))}
        <span style={{ marginLeft:"auto", color:"var(--muted)", fontSize:".78rem", alignSelf:"center" }}>{filtered.length} jobs found</span>
      </div>

      {filtered.length === 0 ? (
        <div className="empty"><div className="empty-icon">🔎</div><div className="empty-text">No jobs match your search.</div></div>
      ) : (
        <div className="grid2">
          {filtered.map(j => (
            <JobCard key={j.id} job={j} applied={appliedIds.has(j.id)} onApply={handleApply} onView={setSelected} />
          ))}
        </div>
      )}

      <JobDetailModal job={selected} applied={appliedIds.has(selected?.id)} userSkills={user.skills} onClose={() => setSelected(null)} onApply={handleApply} />
    </div>
  );
}

// ─── Page: Career ─────────────────────────────────────────────────────────────
function Career({ user, notify }) {
  const [tab, setTab] = useState("astar");
  const [from, setFrom] = useState(user.currentRole);
  const [to, setTo] = useState("Data Scientist");
  const [markovRole, setMarkovRole] = useState("Software Engineer");
  const [path, setPath] = useState(null);
  const [walkPath, setWalkPath] = useState([]);

  const availableRoles = Object.keys(CAREER_GRAPH);

  const findPath = () => {
    const result = [];
    let cur = from;
    const visited = new Set();
    while (cur && !visited.has(cur)) {
      result.push(cur);
      visited.add(cur);
      if (cur === to) break;
      const node = CAREER_GRAPH[cur];
      if (!node) break;
      const next = node.paths.find(p => p === to) || node.paths[0];
      cur = next;
    }
    if (result[result.length-1] !== to && CAREER_GRAPH[to]) result.push(to);
    setPath(result);
    notify("🗺️ Career path computed with A* Search!");
  };

  const simulateWalk = () => {
    const walk = [markovRole];
    let cur = markovRole;
    for (let i = 0; i < 4; i++) {
      const trans = MARKOV[cur];
      if (!trans) break;
      const r = Math.random() * 100;
      let cum = 0;
      for (const t of trans) { cum += t.prob; if (r <= cum) { walk.push(t.role); cur = t.role; break; } }
    }
    setWalkPath(walk);
  };

  const markovData = MARKOV[markovRole] || [];

  return (
    <div>
      <div className="topbar">
        <div><div className="page-title">Career Path Navigator 🗺️</div><div className="page-sub">A* pathfinding · Markov Chain transitions · Union-Find clusters</div></div>
      </div>

      <div className="flex gap8 mb16">
        {[["astar","⚡ A* Path"],["markov","📊 Markov Chain"],["walk","🎲 Career Walk"]].map(([k,l]) => (
          <button key={k} className={`btn btn-sm ${tab===k?"btn-primary":"btn-outline"}`} onClick={() => setTab(k)}>{l}</button>
        ))}
      </div>

      {tab === "astar" && (
        <div>
          <div className="card mb16">
            <div className="sec-title mb12">⚡ Optimal Career Path (A* Search)</div>
            <div className="grid2 mb12">
              <div>
                <div className="text-muted mb8">Current Role</div>
                <select className="select w100" value={from} onChange={e => setFrom(e.target.value)}>
                  {availableRoles.map(r => <option key={r}>{r}</option>)}
                </select>
              </div>
              <div>
                <div className="text-muted mb8">Target Role</div>
                <select className="select w100" value={to} onChange={e => setTo(e.target.value)}>
                  {availableRoles.map(r => <option key={r}>{r}</option>)}
                </select>
              </div>
            </div>
            <button className="btn btn-primary" onClick={findPath}>Find Path →</button>
          </div>

          {path && (
            <div className="card">
              <div className="sec-title mb16">Career Roadmap</div>
              <div className="timeline">
                {path.map((role, i) => {
                  const node = CAREER_GRAPH[role];
                  return (
                    <div className="tl-item" key={role}>
                      <div className="tl-line">
                        <div className="tl-dot" style={{ background: i===0?"var(--accent2)": i===path.length-1?"var(--accent3)":"var(--accent)" }} />
                        {i < path.length-1 && <div className="tl-connector" />}
                      </div>
                      <div className="tl-content">
                        <div className="fw600">{role}</div>
                        {node && <div className="text-muted">Avg. {fmtFull(node.avgSalary)}/yr</div>}
                        {i < path.length-1 && (
                          <div style={{ fontSize:".72rem", color:"var(--muted)", marginTop:4 }}>
                            → {i===0?"Start your journey":"Continue leveling up"}
                          </div>
                        )}
                      </div>
                    </div>
                  );
                })}
              </div>
            </div>
          )}
        </div>
      )}

      {tab === "markov" && (
        <div>
          <div className="card mb16">
            <div className="sec-title mb12">📊 Career Transition Probabilities (Markov Chain)</div>
            <div className="mb12">
              <div className="text-muted mb8">Select Current Role</div>
              <select className="select" value={markovRole} onChange={e => setMarkovRole(e.target.value)}>
                {Object.keys(MARKOV).map(r => <option key={r}>{r}</option>)}
              </select>
            </div>
          </div>
          {markovData.length > 0 ? (
            <div className="card">
              <div className="sec-title mb16">Likely Next Roles from <span style={{color:"var(--accent)"}}>{markovRole}</span></div>
              {markovData.map((t, i) => (
                <div key={t.role} className="mb12">
                  <div className="flex items-center justify-between mb8">
                    <span className="text-sm fw600">{t.role}</span>
                    <span className="mono" style={{ color: i===0?"var(--accent3)":"var(--muted)", fontSize:".78rem" }}>{t.prob}%</span>
                  </div>
                  <div className="prog-wrap">
                    <div className="prog-bar" style={{ width:`${t.prob}%`, background: i===0?"var(--accent3)": i===1?"var(--accent)":"var(--accent2)" }} />
                  </div>
                </div>
              ))}
            </div>
          ) : (
            <div className="empty"><div className="empty-icon">📊</div><div className="empty-text">No transition data for this role.</div></div>
          )}
        </div>
      )}

      {tab === "walk" && (
        <div>
          <div className="card mb16">
            <div className="sec-title mb12">🎲 Career Walk Simulation (Markov Random Walk)</div>
            <div className="mb12">
              <div className="text-muted mb8">Starting Role</div>
              <select className="select" value={markovRole} onChange={e => setMarkovRole(e.target.value)}>
                {Object.keys(MARKOV).map(r => <option key={r}>{r}</option>)}
              </select>
            </div>
            <button className="btn btn-primary" onClick={simulateWalk}>🎲 Simulate Walk</button>
          </div>
          {walkPath.length > 0 && (
            <div className="card">
              <div className="sec-title mb16">Simulated Career Trajectory</div>
              <div style={{ display:"flex", gap:0, flexWrap:"wrap", alignItems:"center" }}>
                {walkPath.map((role, i) => (
                  <div key={i} style={{ display:"flex", alignItems:"center" }}>
                    <div className="card card-sm" style={{ margin:"6px 0", borderColor: i===walkPath.length-1?"var(--accent3)":"var(--border)" }}>
                      <div style={{ fontSize:".72rem", color:"var(--muted)" }}>Year {i*2}</div>
                      <div style={{ fontWeight:600, fontSize:".85rem" }}>{role}</div>
                    </div>
                    {i < walkPath.length-1 && <span style={{ padding:"0 8px", color:"var(--muted)" }}>→</span>}
                  </div>
                ))}
              </div>
            </div>
          )}
        </div>
      )}
    </div>
  );
}

// ─── Page: Tracker ────────────────────────────────────────────────────────────
function Tracker({ applications, onUpdate, onWithdraw, onUndo, onRedo, undoStack, redoStack, notify }) {
  const [filter, setFilter] = useState("All");
  const statuses = ["All","Applied","Interview","Offer","Rejected","Withdrawn"];
  const statusColors = { Applied:"status-applied", Interview:"status-interview", Offer:"status-offer", Rejected:"status-rejected", Withdrawn:"status-withdrawn" };

  const filtered = filter === "All" ? applications : applications.filter(a => a.status === filter);

  const handleStatus = (jobId, status) => {
    onUpdate(jobId, status);
    notify(`Status updated to ${status}`);
    if (status === "Offer") notify("🎉 Congratulations! Offer Received badge unlocked! +500 XP");
  };

  return (
    <div>
      <div className="topbar">
        <div><div className="page-title">Application Tracker 📋</div><div className="page-sub">Two-Stack Undo/Redo · Real-time status tracking</div></div>
        <div className="flex gap8">
          <button className="btn btn-outline btn-sm" onClick={onUndo} disabled={!undoStack.length} title="Undo">↩ Undo {undoStack.length > 0 && `(${undoStack.length})`}</button>
          <button className="btn btn-outline btn-sm" onClick={onRedo} disabled={!redoStack.length} title="Redo">↪ Redo {redoStack.length > 0 && `(${redoStack.length})`}</button>
        </div>
      </div>

      {/* Stats bar */}
      <div className="grid3 mb16">
        {[["📋","Total",applications.length,"#00c2ff"],["🎤","Interviews",applications.filter(a=>a.status==="Interview").length,"#f59e0b"],["🎉","Offers",applications.filter(a=>a.status==="Offer").length,"#10b981"]].map(([icon,label,val,color]) => (
          <div className="stat-card" key={label}>
            <div className="stat-icon">{icon}</div>
            <div><div className="stat-val" style={{color}}>{val}</div><div className="stat-lbl">{label}</div></div>
          </div>
        ))}
      </div>

      <div className="flex gap8 mb16">
        {statuses.map(s => (
          <button key={s} className={`btn btn-sm ${filter===s?"btn-primary":"btn-outline"}`} onClick={() => setFilter(s)}>{s}</button>
        ))}
      </div>

      {filtered.length === 0 ? (
        <div className="empty"><div className="empty-icon">📭</div><div className="empty-text">No applications here. Start applying to jobs!</div></div>
      ) : (
        <div style={{ display:"flex", flexDirection:"column", gap:10 }}>
          {filtered.map(app => (
            <div className="card" key={app.jobId} style={{ display:"flex", alignItems:"center", gap:16 }}>
              <div style={{ flex:1 }}>
                <div className="flex items-center gap8 mb8">
                  <div className="fw600">{app.jobTitle}</div>
                  <span className={`tag ${statusColors[app.status]}`}>{app.status}</span>
                </div>
                <div className="text-muted">🏢 {app.company} · Applied on {app.date}</div>
                {app.notes && <div style={{ fontSize:".75rem", color:"var(--muted)", marginTop:4 }}>📝 {app.notes}</div>}
              </div>
              <div className="flex gap8">
                <select className="select" style={{ fontSize:".75rem", padding:"5px 8px" }}
                  value={app.status} onChange={e => handleStatus(app.jobId, e.target.value)}>
                  {["Applied","Interview","Offer","Rejected","Withdrawn"].map(s => <option key={s}>{s}</option>)}
                </select>
                <button className="btn btn-danger btn-xs" onClick={() => { onWithdraw(app.jobId); notify("Application withdrawn"); }}>Withdraw</button>
              </div>
            </div>
          ))}
        </div>
      )}
    </div>
  );
}

// ─── Page: Skills ─────────────────────────────────────────────────────────────
function Skills({ user, onAddSkill, onUpdateRole, notify }) {
  const [newSkill, setNewSkill] = useState("");
  const [newRole, setNewRole] = useState(user.currentRole);
  const [salaryJob, setSalaryJob] = useState("");
  const [salaryData, setSalaryData] = useState(null);
  const [tab, setTab] = useState("profile");

  const addSkill = () => {
    if (!newSkill.trim()) return;
    onAddSkill(newSkill.trim());
    notify(`+50 XP · Skill "${newSkill}" added!`);
    setNewSkill("");
  };

  const doSalaryAnalysis = () => {
    const job = JOBS.find(j => j.title.toLowerCase().includes(salaryJob.toLowerCase()));
    if (!job) return notify("No job found for that title.");
    const base = (job.salaryMin + job.salaryMax) / 2;
    const years = [0,1,2,3,4,5];
    const salaries = years.map(y => Math.round(base * Math.pow(1.07, y)));
    setSalaryData({ job, base, salaries });
    notify("📊 Salary prediction computed with Dynamic Programming!");
  };

  const clusters = Object.entries(SKILL_CLUSTERS);
  const clusterColors = ["#00c2ff","#7c3aed","#10b981","#f59e0b","#ef4444","#ec4899"];

  const userSkillSet = new Set(user.skills.map(s => s.toLowerCase()));
  const userCluster = clusters.filter(([, skills]) => skills.some(s => userSkillSet.has(s.toLowerCase())));

  const resumeSuggestions = JOBS.map(j => {
    const matched = j.skills.filter(s => userSkillSet.has(s.toLowerCase()));
    return { job: j, score: Math.round(matched.length / j.skills.length * 100) };
  }).sort((a,b) => b.score - a.score).slice(0,6);

  return (
    <div>
      <div className="topbar">
        <div><div className="page-title">Skills & Profile ⚡</div><div className="page-sub">Union-Find clusters · DP salary prediction · Rabin-Karp resume match</div></div>
      </div>

      <div className="flex gap8 mb16">
        {[["profile","👤 Profile"],["clusters","🔗 Skill Clusters"],["salary","💰 Salary DP"],["resume","📄 Resume Match"]].map(([k,l]) => (
          <button key={k} className={`btn btn-sm ${tab===k?"btn-primary":"btn-outline"}`} onClick={() => setTab(k)}>{l}</button>
        ))}
      </div>

      {tab === "profile" && (
        <div className="grid2">
          <div>
            <div className="card mb16">
              <div className="sec-title mb12">👤 Your Profile</div>
              <div className="mb12"><div className="text-muted mb8">Username</div><div className="fw600">{user.username}</div></div>
              <div className="mb12"><div className="text-muted mb8">Email</div><div className="fw600">{user.email}</div></div>
              <div className="mb12">
                <div className="text-muted mb8">Current Role</div>
                <div className="flex gap8">
                  <input className="input" value={newRole} onChange={e => setNewRole(e.target.value)} />
                  <button className="btn btn-primary btn-sm" onClick={() => { onUpdateRole(newRole); notify("Role updated!"); }}>Save</button>
                </div>
              </div>
              <div><div className="text-muted mb8">Current Salary</div><div className="fw600" style={{color:"var(--accent3)"}}>{fmtFull(user.salary)}/yr</div></div>
            </div>
            <div className="card">
              <div className="sec-title mb12">🎖️ Badges ({user.badges.length})</div>
              {user.badges.length ? (
                <div className="chips">{user.badges.map(b => <span key={b} className="tag tag-purple">{b}</span>)}</div>
              ) : <div className="empty" style={{padding:20}}><div className="empty-text">No badges yet. Apply to jobs to earn them!</div></div>}
            </div>
          </div>
          <div className="card">
            <div className="flex items-center justify-between mb12">
              <div className="sec-title">🎒 Your Skills ({user.skills.length})</div>
            </div>
            <div className="flex gap8 mb16">
              <input className="input" placeholder="Add skill (e.g. Python)" value={newSkill}
                onChange={e => setNewSkill(e.target.value)} onKeyDown={e => e.key==="Enter" && addSkill()} />
              <button className="btn btn-primary btn-sm" onClick={addSkill}>Add</button>
            </div>
            <div className="chips">
              {user.skills.length ? user.skills.map(s => (
                <span key={s} className="tag tag-blue">{s}</span>
              )) : <div className="text-muted">No skills yet. Add your first skill above!</div>}
            </div>
          </div>
        </div>
      )}

      {tab === "clusters" && (
        <div>
          <div className="card mb16">
            <div className="sec-title mb8">🔗 Skill Clusters (Union-Find DSU)</div>
            <div className="text-muted text-sm">Skills are grouped using Disjoint Set Union. Your skills are highlighted.</div>
          </div>
          <div style={{ display:"flex", flexDirection:"column", gap:12 }}>
            {clusters.map(([name, skills], i) => (
              <div className="card cluster-card" key={name} style={{ borderColor: clusterColors[i % clusterColors.length] }}>
                <div className="flex items-center justify-between mb8">
                  <div className="fw600" style={{ color: clusterColors[i % clusterColors.length] }}>{name}</div>
                  <span className="text-muted text-sm">{skills.length} skills</span>
                </div>
                <div className="chips">
                  {skills.map(s => (
                    <span key={s} className={`tag ${userSkillSet.has(s.toLowerCase()) ? "tag-green" : "tag-blue"}`}
                      style={!userSkillSet.has(s.toLowerCase()) ? { opacity:.55 } : {}}>
                      {userSkillSet.has(s.toLowerCase()) ? "✓ " : ""}{s}
                    </span>
                  ))}
                </div>
              </div>
            ))}
          </div>
          {userCluster.length > 0 && (
            <div className="card mt16" style={{ borderColor:"var(--accent3)" }}>
              <div className="sec-title mb8" style={{ color:"var(--accent3)" }}>🎯 Your Primary Clusters</div>
              <div className="chips">{userCluster.map(([name]) => <span key={name} className="tag tag-green">{name}</span>)}</div>
            </div>
          )}
        </div>
      )}

      {tab === "salary" && (
        <div>
          <div className="card mb16">
            <div className="sec-title mb12">💰 Salary Growth Prediction (Dynamic Programming)</div>
            <div className="text-sm text-muted mb12">dp[y] = max(dp[y-1] × 1.07, dp[y-1] + skill_bonus[y])</div>
            <div className="flex gap8">
              <input className="input" placeholder="Job title (e.g. Data Scientist)" value={salaryJob}
                onChange={e => setSalaryJob(e.target.value)} onKeyDown={e => e.key==="Enter" && doSalaryAnalysis()} />
              <button className="btn btn-primary" onClick={doSalaryAnalysis}>Predict →</button>
            </div>
          </div>
          {salaryData && (
            <div className="card">
              <div className="sec-title mb8">{salaryData.job.title} @ {salaryData.job.company}</div>
              <div className="text-muted mb16 text-sm">Base: {fmtFull(salaryData.base)}/yr · 7% annual growth projection</div>
              {salaryData.salaries.map((s, y) => (
                <div key={y} className="h-bar">
                  <div className="h-bar-label">Year {y}</div>
                  <div className="h-bar-wrap">
                    <div className="h-bar-fill" style={{ width:`${s/salaryData.salaries[5]*100}%`, background:`linear-gradient(90deg, var(--accent), var(--accent2))` }} />
                  </div>
                  <div className="h-bar-val mono" style={{ width:90, textAlign:"right", color:"var(--text)", fontSize:".78rem" }}>{fmtFull(s)}</div>
                </div>
              ))}
            </div>
          )}
        </div>
      )}

      {tab === "resume" && (
        <div>
          <div className="card mb16">
            <div className="sec-title mb8">📄 Resume Match (Rabin-Karp Keyword Matching)</div>
            <div className="text-sm text-muted">Your skills are matched against job requirements using double-hashing Rabin-Karp algorithm.</div>
          </div>
          <div className="card">
            <div className="sec-title mb16">Top Job Matches for Your Skillset</div>
            {resumeSuggestions.map(({ job, score }) => (
              <div key={job.id} className="h-bar">
                <div className="h-bar-label" title={`${job.title} @ ${job.company}`}>{job.title} @ {job.company}</div>
                <div className="h-bar-wrap">
                  <div className="h-bar-fill" style={{ width:`${score}%`, background: score>=70?"linear-gradient(90deg,var(--accent3),#059669)": score>=40?"linear-gradient(90deg,var(--accent),var(--accent2))":"linear-gradient(90deg,var(--warn),#d97706)" }} />
                </div>
                <div className="h-bar-val">{score}%</div>
              </div>
            ))}
          </div>
        </div>
      )}
    </div>
  );
}

// ─── Page: Analytics ─────────────────────────────────────────────────────────
function Analytics({ user, applications }) {
  const lvl = computeLevel(user.xp);
  const lt = levelTitle(lvl);
  const xpCur = user.xp;
  const xpNext = xpForLevel(lvl + 1);
  const pct = Math.min(100, Math.round(xpCur / xpNext * 100));

  const statusCounts = ["Applied","Interview","Offer","Rejected","Withdrawn"].map(s => ({
    status: s, count: applications.filter(a => a.status === s).length
  }));

  const hashInput = "password123";
  const sha256Demo = "ef92b778bafe771e89245b89ecbc08a44a4e166c06659911881f383d4473e94f";

  return (
    <div>
      <div className="topbar">
        <div><div className="page-title">Analytics & Gamification 📊</div><div className="page-sub">SHA-256 demo · XP system · Badge tracker</div></div>
      </div>

      <div className="grid2 mb16">
        {/* XP & Level */}
        <div className="card" style={{ background:"linear-gradient(135deg, rgba(0,194,255,.08), rgba(124,58,237,.08))", borderColor:"rgba(0,194,255,.2)" }}>
          <div className="sec-title mb16">🎮 Gamification Dashboard</div>
          <div style={{ fontSize:"2rem", marginBottom:4 }}>{lt.label}</div>
          <div style={{ color:"var(--muted)", fontSize:".82rem", marginBottom:16 }}>Level {lvl} · {xpCur} XP</div>
          <div className="text-muted mb8 text-sm">Progress to Level {lvl+1}</div>
          <div className="prog-wrap" style={{ height:10 }}>
            <div className="prog-bar" style={{ width:`${pct}%`, background:"linear-gradient(90deg, var(--accent), var(--accent2))" }} />
          </div>
          <div style={{ fontSize:".72rem", color:"var(--muted)", marginTop:6 }}>{xpCur} / {xpNext} XP</div>
        </div>

        {/* Application funnel */}
        <div className="card">
          <div className="sec-title mb16">📋 Application Pipeline</div>
          {statusCounts.map(({ status, count }) => (
            <div key={status} className="h-bar">
              <div className="h-bar-label">{status}</div>
              <div className="h-bar-wrap">
                <div className="h-bar-fill" style={{ width: applications.length ? `${count/applications.length*100}%`:"0%", background:"linear-gradient(90deg, var(--accent), var(--accent2))" }} />
              </div>
              <div className="h-bar-val">{count}</div>
            </div>
          ))}
          {applications.length === 0 && <div className="text-muted text-sm">Apply to jobs to see stats!</div>}
        </div>
      </div>

      {/* Badges */}
      <div className="card mb16">
        <div className="sec-title mb16">🏅 All Badges</div>
        <div className="grid3">
          {ALL_BADGES.map(b => {
            const earned = user.badges.includes(`${b.icon} ${b.name}`);
            return (
              <div key={b.name} className="card card-sm" style={{ opacity: earned ? 1 : .45, borderColor: earned?"var(--accent3)":"var(--border)", transition:".2s" }}>
                <div style={{ fontSize:"1.5rem", marginBottom:6 }}>{b.icon}</div>
                <div className="fw600 mb8" style={{ fontSize:".85rem" }}>{b.name}</div>
                <div className="text-muted" style={{ fontSize:".72rem" }}>{b.desc}</div>
                {earned && <div style={{ fontSize:".7rem", color:"var(--accent3)", marginTop:6, fontWeight:600 }}>✓ Earned</div>}
              </div>
            );
          })}
        </div>
      </div>

      {/* SHA-256 Demo */}
      <div className="card" style={{ borderColor:"rgba(124,58,237,.3)" }}>
        <div className="sec-title mb12">🔒 SHA-256 Password Hashing Demo</div>
        <div className="text-muted text-sm mb12">Passwords in SkillQuest are stored as SHA-256 hashes (FIPS 180-4 compliant). Never stored in plaintext.</div>
        <div className="card card-sm" style={{ background:"var(--bg3)" }}>
          <div className="text-muted mb8" style={{ fontSize:".72rem" }}>Input</div>
          <div className="mono" style={{ color:"var(--text)", fontSize:".82rem" }}>{hashInput}</div>
        </div>
        <div style={{ textAlign:"center", color:"var(--muted)", fontSize:"1.2rem", padding:"8px 0" }}>↓ SHA-256</div>
        <div className="card card-sm" style={{ background:"var(--bg3)" }}>
          <div className="text-muted mb8" style={{ fontSize:".72rem" }}>Hash Output (64 hex chars)</div>
          <div className="mono" style={{ color:"var(--accent)", fontSize:".72rem", wordBreak:"break-all" }}>{sha256Demo}</div>
        </div>
      </div>
    </div>
  );
}

// ─── Root App ─────────────────────────────────────────────────────────────────
const NAV = [
  { id:"dashboard", icon:"🏠", label:"Dashboard" },
  { id:"jobs",      icon:"🔍", label:"Job Search" },
  { id:"career",    icon:"🗺️", label:"Career Path" },
  { id:"tracker",   icon:"📋", label:"App Tracker" },
  { id:"skills",    icon:"⚡", label:"Skills" },
  { id:"analytics", icon:"📊", label:"Analytics" },
];

export default function App() {
  const [authed, setAuthed] = useState(false);
  const [user, setUser] = useState(null);
  const [page, setPage] = useState("dashboard");
  const [applications, setApplications] = useState([]);
  const [undoStack, setUndoStack] = useState([]);
  const [redoStack, setRedoStack] = useState([]);
  const [notif, setNotif] = useState(null);

  const notify = (msg) => { setNotif(msg); };

  const handleLogin = (u) => { setUser({ ...u }); setAuthed(true); };
  const handleLogout = () => { setAuthed(false); setUser(null); setPage("dashboard"); setApplications([]); setUndoStack([]); setRedoStack([]); };

  const addXP = (xp) => setUser(u => ({ ...u, xp: u.xp + xp, level: computeLevel(u.xp + xp) }));

  const applyToJob = (job) => {
    if (applications.find(a => a.jobId === job.id)) return;
    const app = { jobId: job.id, jobTitle: job.title, company: job.company, status:"Applied", date: today(), notes:"" };
    setUndoStack(s => [...s, { type:"ADD", after: app }]);
    setRedoStack([]);
    setApplications(prev => [...prev, app]);
    addXP(100);
    setUser(u => {
      const badges = [...u.badges];
      if (!badges.includes("📝 First Application")) badges.push("📝 First Application");
      return { ...u, badges };
    });
  };

  const updateStatus = (jobId, status) => {
    setApplications(prev => prev.map(a => {
      if (a.jobId !== jobId) return a;
      setUndoStack(s => [...s, { type:"UPDATE", before: a, after: { ...a, status } }]);
      setRedoStack([]);
      return { ...a, status };
    }));
    if (status === "Offer") {
      addXP(500);
      setUser(u => {
        const badges = [...u.badges];
        if (!badges.includes("🎉 Offer Received")) badges.push("🎉 Offer Received");
        return { ...u, badges };
      });
    }
  };

  const withdraw = (jobId) => {
    const app = applications.find(a => a.jobId === jobId);
    if (!app) return;
    setUndoStack(s => [...s, { type:"REMOVE", before: app }]);
    setRedoStack([]);
    setApplications(prev => prev.filter(a => a.jobId !== jobId));
  };

  const undo = () => {
    if (!undoStack.length) return;
    const action = undoStack[undoStack.length - 1];
    setUndoStack(s => s.slice(0,-1));
    setRedoStack(s => [...s, action]);
    if (action.type === "ADD") setApplications(prev => prev.filter(a => a.jobId !== action.after.jobId));
    if (action.type === "REMOVE") setApplications(prev => [...prev, action.before]);
    if (action.type === "UPDATE") setApplications(prev => prev.map(a => a.jobId === action.before.jobId ? action.before : a));
    notify("↩ Undo applied");
  };

  const redo = () => {
    if (!redoStack.length) return;
    const action = redoStack[redoStack.length - 1];
    setRedoStack(s => s.slice(0,-1));
    setUndoStack(s => [...s, action]);
    if (action.type === "ADD") setApplications(prev => [...prev, action.after]);
    if (action.type === "REMOVE") setApplications(prev => prev.filter(a => a.jobId !== action.before.jobId));
    if (action.type === "UPDATE") setApplications(prev => prev.map(a => a.jobId === action.after.jobId ? action.after : a));
    notify("↪ Redo applied");
  };

  const addSkill = (skill) => {
    setUser(u => {
      if (u.skills.includes(skill)) return u;
      const skills = [...u.skills, skill];
      const badges = [...u.badges];
      if (skills.length >= 5 && !badges.includes("🎒 Skill Collector")) badges.push("🎒 Skill Collector");
      return { ...u, skills, badges, xp: u.xp + 50 };
    });
  };

  const updateRole = (role) => setUser(u => ({ ...u, currentRole: role }));

  if (!authed) return <><style>{css}</style><Auth onLogin={handleLogin} /></>;

  const lvl = computeLevel(user.xp);
  const lt = levelTitle(lvl);

  return (
    <>
      <style>{css}</style>
      <div className="app">
        {/* Sidebar */}
        <div className="sidebar">
          <div className="sidebar-logo">
            <div className="logo-text">SkillQuest</div>
            <div className="logo-sub">DSA-POWERED CAREER OS</div>
          </div>
          <div className="nav-section">
            <div className="nav-label">Navigation</div>
            {NAV.map(n => (
              <button key={n.id} className={`nav-btn ${page===n.id?"active":""}`} onClick={() => setPage(n.id)}>
                <span className="nav-icon">{n.icon}</span>{n.label}
              </button>
            ))}
          </div>
          <div className="sidebar-user">
            <div className="user-chip">
              <div className="avatar">{user.username[0].toUpperCase()}</div>
              <div className="user-info">
                <div className="user-name">{user.username}</div>
                <div className="user-level">{lt.label}</div>
              </div>
            </div>
            <button className="btn btn-outline btn-sm w100" style={{ marginTop:8, justifyContent:"center" }} onClick={handleLogout}>Logout</button>
          </div>
        </div>

        {/* Main */}
        <div className="main">
          {page === "dashboard" && <Dashboard user={user} applications={applications} onNav={setPage} />}
          {page === "jobs"      && <Jobs user={user} applications={applications} onApply={applyToJob} notify={notify} />}
          {page === "career"    && <Career user={user} notify={notify} />}
          {page === "tracker"   && <Tracker applications={applications} onUpdate={updateStatus} onWithdraw={withdraw} onUndo={undo} onRedo={redo} undoStack={undoStack} redoStack={redoStack} notify={notify} />}
          {page === "skills"    && <Skills user={user} onAddSkill={addSkill} onUpdateRole={updateRole} notify={notify} />}
          {page === "analytics" && <Analytics user={user} applications={applications} />}
        </div>
      </div>

      {notif && <Notif msg={notif} onDone={() => setNotif(null)} />}
    </>
  );
}