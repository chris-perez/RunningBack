# RunningBack

###Workflow###
1. Create new branch
2. Commit changes often to your branch
3. Submit a pull request when all your changes are ready to be merged
4. Someone else should review and accept the pull request

### Git Commands ###
Clone Repo
```
git clone <GitHub url>
```

Checkout New Branch
```
Create new branch on GitHub.
git checkout -b <branch_name>                                            // Creates new branch locally
git branch --set-upstream-to=origin/<branch_name> <branch_name>         //links local branch to remote branch
```

Pull Remote Changes
```
git pull
```

Add New File to Branch Locally
```
git add <file_name>
```

Add All New Files to Branch Locally
```
git add -A
```

Commit Changes Locally
```
git commit -m "commit message"
```

Push Changes to Remote Branch
```
git push
```
