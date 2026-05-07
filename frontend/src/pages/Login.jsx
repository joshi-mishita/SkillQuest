import { useState } from "react";
import users from "../data/users";

function Login({ onAuth }) {
  const [mode, setMode] = useState("login");
  const [error, setError] = useState("");

  const [form, setForm] = useState({
    username: "",
    password: "",
    email: "",
    role: "",
    salary: "",
  });

  const updateField = (key, value) => {
    setForm((prev) => ({
      ...prev,
      [key]: value,
    }));
  };

  const handleSubmit = (e) => {
    e.preventDefault();
    setError("");

    if (!form.username || !form.password) {
      setError("Fill all required fields");
      return;
    }

    if (mode === "login") {
      const existing = users[form.username];

      if (!existing) {
        setError("User not found");
        return;
      }

      if (existing.password !== form.password) {
        setError("Incorrect password");
        return;
      }

      onAuth(existing);
      return;
    }

    if (!form.role || !form.salary) {
      setError("Role and salary are required");
      return;
    }

    onAuth({
      username: form.username,
      email: form.email || `${form.username}@mail.com`,
      currentRole: form.role,
      salary: Number(form.salary),
      skills: [],
      appliedJobIds: [],
      savedJobIds: [],
      xp: 0,
    });
  };

  return (
    <div className="auth-page">
      <div className="auth-card">
        <div className="brand">
          <h1>
            <span>Skill</span>Quest
          </h1>
          <p>Build skills. Track progress. Discover opportunities.</p>
        </div>

        <div className="toggle-box">
          <button
            type="button"
            className={mode === "login" ? "active" : ""}
            onClick={() => {
              setMode("login");
              setError("");
            }}
          >
            Login
          </button>

          <button
            type="button"
            className={mode === "register" ? "active" : ""}
            onClick={() => {
              setMode("register");
              setError("");
            }}
          >
            Register
          </button>
        </div>

        <form onSubmit={handleSubmit}>
          {mode === "register" && (
            <>
              <input
                type="email"
                placeholder="Email"
                value={form.email}
                onChange={(e) =>
                  updateField("email", e.target.value)
                }
              />

              <input
                type="text"
                placeholder="Current Role"
                value={form.role}
                onChange={(e) =>
                  updateField("role", e.target.value)
                }
              />

              <input
                type="number"
                placeholder="Current Salary"
                value={form.salary}
                onChange={(e) =>
                  updateField("salary", e.target.value)
                }
              />
            </>
          )}

          <input
            type="text"
            placeholder="Username"
            value={form.username}
            onChange={(e) =>
              updateField("username", e.target.value)
            }
          />

          <input
            type="password"
            placeholder="Password"
            value={form.password}
            onChange={(e) =>
              updateField("password", e.target.value)
            }
          />

          {error && (
            <p
              style={{
                color: "#ff7a7a",
                fontSize: ".9rem",
              }}
            >
              {error}
            </p>
          )}

          <button type="submit" className="submit-btn">
            {mode === "login" ? "Login" : "Create Account"}
          </button>
        </form>

        <p
          style={{
            marginTop: 20,
            color: "#9ba7c2",
            fontSize: ".85rem",
            textAlign: "center",
          }}
        >
          Demo login: alice / password123
        </p>
      </div>
    </div>
  );
}

export default Login;