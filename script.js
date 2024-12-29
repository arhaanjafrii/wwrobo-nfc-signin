function doGet(e) {
  const ss = SpreadsheetApp.openById("1StFnvcY1iEZSvK03KPp1Dgg6-fc0g1l-4m3B1Sexdks"); // Replace with your actual Google Sheets ID
  const userDataSheet = ss.getSheetByName("User_Data");
  const attendanceSheet = ss.getSheetByName("Attendance");

  if (!e.parameter.mode || !e.parameter.uid) {
    return ContentService.createTextOutput("Missing mode or UID parameter.");
  }

  const mode = e.parameter.mode;
  const uid = e.parameter.uid;

  if (mode === "register") {
    const name = e.parameter.name;
    if (!name) {
      return ContentService.createTextOutput("Name is required for registration.");
    }

    // Check if UID already exists in User_Data
    const userData = userDataSheet.getDataRange().getValues();
    const existingUser = userData.find(row => row[0] === uid);

    if (existingUser) {
      return ContentService.createTextOutput("UID already registered.");
    }

    // Add UID and name to User_Data
    userDataSheet.appendRow([uid, name]);
    return ContentService.createTextOutput("User registered successfully.");
  } else if (mode === "attendance") {
    const userData = userDataSheet.getDataRange().getValues();
    const user = userData.find(row => row[0] === uid);

    if (!user) {
      return ContentService.createTextOutput("UID not found in User_Data.");
    }

    const name = user[1];
    const timestamp = new Date();
    const status = determineStatus(uid, attendanceSheet);

    // Log attendance
    attendanceSheet.appendRow([timestamp, uid, name, status]);
    return ContentService.createTextOutput(`Attendance logged for ${name} as ${status}.`);
  }

  return ContentService.createTextOutput("Invalid mode.");
}

// Helper function to determine if user is signing in or out
function determineStatus(uid, attendanceSheet) {
  const attendanceData = attendanceSheet.getDataRange().getValues();
  const userLogs = attendanceData.filter(row => row[1] === uid);

  if (userLogs.length === 0 || userLogs[userLogs.length - 1][3] === "Signed Out") {
    return "Signed In";
  } else {
    return "Signed Out";
  }
}
