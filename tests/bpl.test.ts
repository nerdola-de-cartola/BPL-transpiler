import { expect, test } from "vitest";
import { readFile, unlink } from "node:fs/promises";
import { execa } from "execa";

const tests = [
  {
    inputPath: "./array/t1.bpl",
    outputPath: "./array/t1.s",
    cFilePath: "./array/t1.c",
    expectedExitCode: 0,
  },
  {
    inputPath: "./array/t2.bpl",
    outputPath: "./array/t2.s",
    cFilePath: "./array/t2.c",
    expectedExitCode: 0,
  },
  {
    inputPath: "./assignment/t1.bpl",
    outputPath: "./assignment/t1.s",
    cFilePath: "./assignment/t1.c",
    expectedExitCode: 0,
  },
  {
    inputPath: "./conditions/t1.bpl",
    outputPath: "./conditions/t1.s",
    cFilePath: "./conditions/t1.c",
    expectedExitCode: 0,
  },
  {
    inputPath: "./functions/t1.bpl",
    outputPath: "./functions/t1.s",
    cFilePath: "./functions/t1.c",
    expectedExitCode: 96,
  },
  {
    inputPath: "./functions/t2.bpl",
    outputPath: "./functions/t2.s",
    cFilePath: "./functions/t2.c",
    expectedExitCode: 5,
  },
  {
    inputPath: "./variables/t1.bpl",
    outputPath: "./variables/t1.s",
    cFilePath: "./variables/t1.c",
    expectedExitCode: 113,
  },
  {
    inputPath: "./variables/t2.bpl",
    outputPath: "./variables/t2.s",
    cFilePath: "./variables/t2.c",
    expectedExitCode: 76,
  },
];

test.each(tests)(
  "it generates the expected assembly output for: %s",
  async ({ inputPath, outputPath, cFilePath, expectedExitCode }) => {
    try {
      // compilation test
      console.log(`Testing ${inputPath}...`);
      //const expectedOutput = await readFile(outputPath, "utf-8");
      const { exitCode: compilationExitCode } = await execa("./compiler", [
        inputPath,
        `${outputPath}.tmp.s`,
      ]);
      //const compilerOutput = await readFile(`${outputPath}.tmp.s`, "utf-8");
      //expect(compilerOutput).toBe(expectedOutput);
      expect(compilationExitCode).toBe(0);

      // execution test
      const { exitCode: gccExitCode } = await execa("gcc", [
        "-o",
        `${outputPath}.exe`,
        `${outputPath}.tmp.s`,
        `${cFilePath}`,
      ]);
      expect(gccExitCode).toBe(0);
      const { exitCode: executionExitCode } = await execa(`${outputPath}.exe`, {
        reject: false,
      });
      expect(executionExitCode).toBe(expectedExitCode);
    } catch (error) {
      // cleanup
      throw error;
    } finally {
      try {
        await unlink(`${outputPath}.tmp.s`);
        await unlink(`${outputPath}.exe`);
      } catch (error) {
        console.log("error unlinking");
      }
    }
  }
);
