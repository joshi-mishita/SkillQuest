import { useEffect, useState } from "react";

import Login from "./pages/Login";
import Dashboard from "./pages/Dashboard";
import Jobs from "./pages/Jobs";
import Career from "./pages/Career";
import Tracker from "./pages/Tracker";
import Skills from "./pages/Skills";
import Resume from "./pages/Resume";

import {
  loadUser,
  saveUser,
  clearUser,
} from "./utils/storage";

function App() {
  const [user, setUser] = useState(loadUser());
  const [page, setPage] = useState("dashboard");

  useEffect(() => {
    if (user) {
      saveUser(user);
    }
  }, [user]);

  const handleAuth = (userData) => {
    setUser(userData);
    setPage("dashboard");
  };

  const handleLogout = () => {
    clearUser();
    setUser(null);
    setPage("dashboard");
  };

  const updateUser = (updates) => {
    setUser((prev) => ({
      ...prev,
      ...updates,
    }));
  };

  if (!user) {
    return <Login onAuth={handleAuth} />;
  }

  const sharedProps = {
    user,
    setPage,
    handleLogout,
    updateUser,
  };

  if (page === "jobs") {
    return <Jobs {...sharedProps} />;
  }

  if (page === "career") {
    return <Career {...sharedProps} />;
  }

  if (page === "tracker") {
    return <Tracker {...sharedProps} />;
  }

  if (page === "skills") {
    return <Skills {...sharedProps} />;
  }

  if (page === "resume") {
    return <Resume {...sharedProps} />;
  }

  return (
    <Dashboard
      user={user}
      setPage={setPage}
      onLogout={handleLogout}
    />
  );
}

export default App;