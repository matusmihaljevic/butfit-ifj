# Git survival guide

When you find yourself lost and desperate with **Git**...

## Table of contents

- [Introduction](#introduction)
    - [What is Git?](#what-is-git)
    - [Git keywords](#git-keywords)
    - [Snapshots vs. Deltas](#snapshots-vs-deltas)
    - [Git basics](#git-basics)
- [Tips](#tips)
    - [Initialize environment](#initialize-environment)
    - [Aliases](#aliases)
- [This project](#for-this-project)
    - [Worklow rules](#workflow-rules)
    - [Creating a merge request](#creating-a-merge-request-mr)
    - [Commit strategies](#commit-strategies)

## Introduction

### What is Git?

Git is a *distributed version control system* that allows multiple people to work on a project simultaneously without overwriting each other's changes. It is designed to handle everything from small to very large projects with speed and efficiency.


### Git keywords

- **Repository (repo)**: A directory that contains your project files and the entire version history of those files.
- **Commit**: A *snapshot* of your project at a point in time. Each commit has a unique **hash**.
- **Branch**: A parallel version of a repository. It allows you to work on different features or fixes separately.
- **Merge**: Combining changes from different branches into one.
- **Clone**: A copy of a repository.
- **Pull**: Fetching changes from a remote repository to your local repository.
- **Push**: Sending your changes to a remote repository.
- **Remote**: A version of your project that is hosted on the internet or network.


### Snapshots vs. Deltas

Unlike other version control systems that store changes as file differences (systemsdeltas), Git takes a snapshot of the entire project at each commit. If files haven’t changed, Git doesn’t store the file again, just a link to the previous identical file.


### Git Basics

#### pull
**`git pull <remote-name> <branch-name>`** <br>
Pull changes from remote. 
Incorporates changes from a remote repository into the current branch.
`git pull` is basically `git fetch` + `git merge`. Keep that in mind.

#### fetch
**`git fetch <remote-name>`** <br>
Download objects and refs from another repository.
Downloads commits, files, and references from a remote repository into your local repository. However, it does not integrate these changes into your working directory or current branch.

#### status
**`git status`** <br>
Show the working tree status.

#### log
**`git log`** <br>
Show commit logs.

#### add
**`git add <file-name>`** <br>
Add changes to staging area.
This command updates the index using the current content found in the working tree, to prepare the content staged for the next commit.

#### commit
**`git commit`** <br>
Creates snapshot of your project's current state.
Crucial part of the version control process, allowing you to save snapshots of your project at various points in time.

#### merge
**`git merge <branch-name>`** <br>
Combines the changes from one branch into another. The commit history of both branches remains intact.

#### rebase
**`git rebase <branch-name>`** <br>
Replays commits from the current branch onto another base commit, creating a linear history.
Interactive rebase allows you to edit, reorder, squash, or drop commits during the rebase process.

#### push
**`git push <remote-name> <branch-name>`** <br>
Transfers commits from your local repository to a remote repository, updating the remote branches with changes made locally.
`<remote-name>` is typically origin, which is the default name for the remote repository created when you clone a repository.

#### reflog
**`git reflog`** <br>
Displays a list of actions and changes that have affected the HEAD reference.
Particularly useful for recovering from mistakes, as it allows you to find and reference previous states of the repository that might otherwise be difficult to locate.


## Tips

### Initialize environment

##### Identity:
- **`git config --global user.name "Jack Sparrow"`**
- **`git config --global user.email "jack.sparrow@example.org"`**

##### Pull
- **`git config --global pull.rebase true`** - use rebase instead of merge by default. When pulling changes from a remote branch, this option replays your local commits on top of the fetched commits, resulting in a linear project history.

##### Appearance
- **`git config --global color.ui true`** - enable colored output.
- **`git config --global grep.lineNumber true`** - show line numbers by default when using `git grep`.

##### Merge
- **`git config --global merge.ff false`** - disables fast-forward merges (it's better, trust me).
- **`git config --global merge.stat true`** - enable the display of a diffstat (a summary of changes) when performing a merge.
- **`git config --global merge.summary true`** - add a summary of merged commits in the merge commit message.

##### Resolving conflicts
- **`git config --global rerere.enabled true`** - enable the **"reuse recorded resolution"**(*rerere*) feature, which helps to remember how you resolved conflicts and reuse that resolution in the future if the same conflict arises again.

##### Diff
- **`git config --global diff.renames copies`** - configure Git to detect renames and copies in `git diff`. It helps in tracking changes more accurately, especially when files are renamed or copied.

### Aliases
Custom shortcuts for Git commands. They allow you to define shorter or more convenient names for frequently used commands, saving time and reducing typing effort.

- `git config --global alias.co checkout`
- `git config --global alias.ci "commit -v"`
- `git config --global alias.amend "commit --amend -v"`
- `git config --global alias.br branch`
- `git config --global alias.st status`
- `git config --global alias.cp cherry-pick`
- `git config --global alias.ff "merge --ff"`
- `git config --global alias.l "log --oneline --decorate"`
- `git config --global alias.last "log -1 --stat"`
- `git config --global alias.g "log --graph --oneline --decorate --all"`


## For this project

Everything you need to know for this project can be found here.

### Workflow rules

To maintain a clean and efficient Git workflow follow these guidelines:

- **No direct pushes to `main`**: Changes to the `main` branch are only integrated via merge requests (MRs) after review and approval.
- **Merge requests (MRs)**: Every new feature or bug fix is developed in its own branch and submitted via a merge request.
- **Feature-based merge requests**: Each merge request should handle only **one single feature** or fix. This ensures that changes are isolated and easier to review.
- **Approval process**: Every MR must be reviewed and approved by at least one team member before merging into `main`.


### Creating a Merge Request (MR)

1. When working on a new feature, always create a dedicated branch:
`git checkout -b <branch-name>`

2. Push your feature branch to the remote repository:
`git push origin <branch-name>`

3. Navigate to Github and create a merge request from your branch into `main`. There should also be a direct link after the first push.

4. Provide a clear description of the changes introduced and ensure the MR only handles one feature.

5. Request approval from your team members.


### Commit strategies

##### Amend
To modify the most recent commit (e.g., to fix a typo or add missing changes), add files to staging area and use:
`git commit --amend`

This allows you to adjust the last commit without adding a new one. Remember to force push if the commit has already been pushed:
`git push --force-with-lease`

##### Fixup
If you have multiple commits and realize you need to fix an older commit, use fixup:
`git commit --fixup=<commit-hash>`

This creates a new commit that can later be combined (squashed) with the target commit during an interactive rebase.