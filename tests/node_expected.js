const fs = require("node:fs");
const path = require("node:path");

const filePath = path.join(__dirname, "ca.crt");
const ca = fs.readFileSync(filePath);
const cert = ca.toString();
const jsonValue = JSON.stringify(cert.trim());

const outPath = path.join(__dirname, "expected_from_node.txt");
fs.writeFileSync(outPath, jsonValue, "utf8");
